#include "playermodel.hpp"

/**
 * @brief Returns the number of rows in this model
 * @return The row count
 */
int PlayerModel::rowCount(const QModelIndex &) const {
	return static_cast<int>(m_data.size());
}

/**
 * @brief Returns data in this model
 * @param index The index of the data to return
 * @param role The data in question
 * @return The data
 */
QVariant PlayerModel::data(const QModelIndex &index, int role) const {
	const std::unique_ptr<Curver> &curver = m_data[static_cast<unsigned long>(index.row())];
	switch (role) {
	case NameRole:
		return curver->userName;
	case ColorRole:
		return curver->getColor();
	case LeftKeyRole:
		return curver->getLeftKey();
	case RightKeyRole:
		return curver->getRightKey();
	case RoundScoreRole:
		return curver->roundScore;
	case TotalScoreRole:
		return curver->totalScore;
	case ControllerRole:
		return static_cast<int>(curver->controller);
	case PingRole:
		return curver->ping;
	default:
		return "Unknown role";
	}
}

/**
 * @brief Returns the role names
 * @return Role names
 */
QHash<int, QByteArray> PlayerModel::roleNames() const {
	return m_roleNames;
}

/**
 * @brief Appends a new player to this model
 */
void PlayerModel::appendPlayer() {
	beginResetModel();
	m_data.push_back(std::make_unique<Curver>(rootNode));
	m_data.back()->userName = "Player " + QString::number(m_data.size());
	connect(m_data.back().get(), &Curver::died, this, &PlayerModel::processDeath);
	endResetModel();
	playerModelChanged();
}

/**
 * @brief Appends a new bot to this model
 */
void PlayerModel::appendBot() {
	appendPlayer();
	setController(m_data.size() - 1, static_cast<int>(Curver::Controller::CONTROLLER_BOT));
}

/**
 * @brief Removes a player from this model
 * @param row The index of the player
 */
void PlayerModel::removePlayer(int row) {
	beginResetModel();
	m_data.erase(m_data.begin() + row);
	endResetModel();
	playerModelChanged();
}

/**
 * @brief Removes a player from this model
 * @param curver The pointer to the player
 */
void PlayerModel::removeCurver(Curver *curver) {
	auto it = std::ranges::find_if(m_data, [=](const auto &c) { return c.get() == curver; });
	if (it != m_data.cend()) {
		removePlayer(it - m_data.cbegin());
	}
}

/**
 * @brief Sets the color of a player
 * @param row The index of the player
 * @param color The new color
 */
void PlayerModel::setColor(int row, QColor color) {
	m_data[static_cast<unsigned long>(row)]->setColor(color);
	playerModelChanged();
}

/**
 * @brief Sets the left key of a player
 * @param row The index of the player
 * @param key The new key
 */
void PlayerModel::setLeftKey(int row, Qt::Key key) {
	const auto player = m_data[static_cast<unsigned long>(row)].get();
	player->setLeftKey(key);
	Gui::getSingleton().postInfoBar("Set left key of " + player->userName + " to " + QKeySequence(key).toString());
}

/**
 * @brief Sets the right key of a player
 * @param row The index of the player
 * @param key The new key
 */
void PlayerModel::setRightKey(int row, Qt::Key key) {
	const auto player = m_data[static_cast<unsigned long>(row)].get();
	player->setRightKey(key);
	Gui::getSingleton().postInfoBar("Set right key of " + player->userName + " to " + QKeySequence(key).toString());
}

/**
 * @brief Sets the username of a player
 * @param row The index of the player
 * @param username The new username
 */
void PlayerModel::setUserName(int row, QString username) {
	m_data[static_cast<unsigned long>(row)]->userName = username;
	dataChanged(index(row, 0), index(row, 0), QVector<int>() = {NameRole});
	playerModelChanged();
}

/**
 * @brief Sets the controller of a player
 * @param row The index of the player
 * @param ctrl The new controller
 */
void PlayerModel::setController(int row, int ctrl) {
	m_data[static_cast<unsigned long>(row)]->controller = static_cast<Curver::Controller>(ctrl);
	dataChanged(index(row, 0), index(row, 0), QVector<int>() = {ControllerRole});
	playerModelChanged();
}

/**
 * @brief Sets the root node of the scene graph
 * @param rootNode The new root node
 */
void PlayerModel::setRootNode(QSGNode *rootNode) {
	this->rootNode = rootNode;
}

/**
 * @brief Returns all players as a Curver vector
 * @return All players
 */
std::vector<std::unique_ptr<Curver>> &PlayerModel::getCurvers() {
	return this->m_data;
}

/**
 * @brief Serializes this PlayerModel
 * @param out The stream to serialize into
 */
void PlayerModel::serialize(QDataStream &out) const {
	out << static_cast<unsigned>(m_data.size());
	for (unsigned long i = 0; i < m_data.size(); ++i) {
		const std::unique_ptr<Curver> &c = m_data[i];
		out << c->userName << c->getColor() << c->roundScore << c->totalScore << static_cast<uint8_t>(c->controller) << static_cast<uint8_t>(c->isAlive());
	}
}

/**
 * @brief Parses a PlayerModel from a stream
 * @param in The stream to parse from
 */
void PlayerModel::parse(QDataStream &in) {
	beginResetModel();
	unsigned size;
	in >> size;
	m_data.resize(size);
	for (auto &c : m_data) {
		if (!c) {
			c = std::make_unique<Curver>(rootNode);
		}
		QColor color;
		uint8_t ctrl, isAlive;
		in >> c->userName >> color >> c->roundScore >> c->totalScore >> ctrl >> isAlive;
		c->setColor(color);
		c->controller = static_cast<Curver::Controller>(ctrl);
		c->setAlive(static_cast<bool>(isAlive));
	}
	endResetModel();
}

/**
 * @brief Returns the last player added
 * @return The last added Curver
 */
Curver *PlayerModel::getNewPlayer() {
	appendPlayer();
	return m_data.back().get();
}

/**
 * @brief Forces a refresh of the GUI
 */
void PlayerModel::forceRefresh() {
	beginResetModel();
	endResetModel();
	playerModelChanged();
}

/**
 * @brief Forces a refresh of the score roles in the GUI
 */
void PlayerModel::processDeath() {
	curverDied();
	dataChanged(index(0, 0), index(static_cast<int>(m_data.size()) - 1, 0), QVector<int>() = {RoundScoreRole, TotalScoreRole});
	playerModelChanged();
}

/**
 * @brief Removes all bots
 */
void PlayerModel::removeBots() {
	for (size_t i = 0; i < m_data.size(); ++i) {
		if (m_data[i]->controller == Curver::Controller::CONTROLLER_BOT) {
			this->removePlayer(i);
		}
	}
}
