#include "playermodel.h"

PlayerModel::PlayerModel(QObject *parent) : QAbstractListModel(parent)
{
	// role names
	m_roleNames[NameRole] = "name";
	m_roleNames[ColorRole] = "color";
	m_roleNames[LeftKeyRole] = "leftKey";
	m_roleNames[RightKeyRole] = "rightKey";
	m_roleNames[RoundScoreRole] = "roundScore";
	m_roleNames[TotalScoreRole] = "totalScore";
	m_roleNames[ControllerRole] = "controller";
}

int PlayerModel::rowCount(const QModelIndex &) const
{
	return static_cast<int>(m_data.size());
}

QVariant PlayerModel::data(const QModelIndex &index, int role) const
{
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
		return curver->controller;
	default:
		return "Unknown role";
	}
}

QHash<int, QByteArray> PlayerModel::roleNames() const
{
	return m_roleNames;
}

PlayerModel &PlayerModel::getSingleton()
{
	static PlayerModel singleton;
	return singleton;
}

void PlayerModel::appendPlayer()
{
	beginResetModel();
	m_data.push_back(std::make_unique<Curver>(rootNode));
	m_data.back()->userName = "Player " + QString::number(m_data.size());
	connect(m_data.back().get(), SIGNAL(died()), this, SLOT(processDeath()));
	endResetModel();
	emit playerModelChanged();
}

void PlayerModel::removePlayer(int row)
{
	beginResetModel();
	m_data.erase(m_data.begin() + row);
	endResetModel();
	emit playerModelChanged();
}

void PlayerModel::setColor(int row, QColor color)
{
	m_data[static_cast<unsigned long>(row)]->setColor(color);
	emit playerModelChanged();
}

void PlayerModel::setLeftKey(int row, Qt::Key key)
{
	m_data[static_cast<unsigned long>(row)]->setLeftKey(key);
	emit Gui::getSingleton().postInfoBar("Set left key to " + QKeySequence(key).toString());
}

void PlayerModel::setRightKey(int row, Qt::Key key)
{
	m_data[static_cast<unsigned long>(row)]->setRightKey(key);
	emit Gui::getSingleton().postInfoBar("Set right key to " + QKeySequence(key).toString());
}

void PlayerModel::setUserName(int row, QString username)
{
	m_data[static_cast<unsigned long>(row)]->userName = username;
	emit dataChanged(index(row, 0), index(row, 0), QVector<int>() = {NameRole});
	emit playerModelChanged();
}

void PlayerModel::setController(int row, int ctrl)
{
	m_data[static_cast<unsigned long>(row)]->controller = static_cast<Curver::Controller>(ctrl);
	emit dataChanged(index(row, 0), index(row, 0), QVector<int>() = {ControllerRole});
	emit playerModelChanged();
}

void PlayerModel::setRootNode(QSGNode *rootNode)
{
	this->rootNode = rootNode;
}

std::vector<std::unique_ptr<Curver> > &PlayerModel::getCurvers()
{
	return this->m_data;
}

void PlayerModel::serialize(QDataStream &out) const
{
	out << static_cast<unsigned>(m_data.size());
	for (unsigned long i = 0; i < m_data.size(); ++i) {
		const std::unique_ptr<Curver> &c = m_data[i];
		out << c->userName << c->getColor() << c->roundScore << c->totalScore << static_cast<uint8_t>(c->controller);
	}
}

void PlayerModel::parse(QDataStream &in)
{
	emit beginResetModel();
	unsigned size;
	in >> size;
	m_data.resize(size);
	for (auto &c : m_data) {
		if (!c) {
			c = std::make_unique<Curver>(rootNode);
		}
		QColor color;
		uint8_t ctrl;
		in >> c->userName >> color >> c->roundScore >> c->totalScore >> ctrl;
		c->setColor(color);
		c->controller = static_cast<Curver::Controller>(ctrl);
	}
	emit endResetModel();
}

Curver *PlayerModel::getNewPlayer()
{
	appendPlayer();
	return m_data.back().get();
}

void PlayerModel::forceRefresh()
{
	emit beginResetModel();
	emit endResetModel();
	emit playerModelChanged();
}

void PlayerModel::processDeath()
{
	emit curverDied();
	emit dataChanged(index(0, 0), index(static_cast<int>(m_data.size()) - 1, 0), QVector<int>() = {RoundScoreRole, TotalScoreRole});
	emit playerModelChanged();
}
