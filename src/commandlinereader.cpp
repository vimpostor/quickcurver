#ifdef HEADLESS_SERVER
#include "commandlinereader.h"

/**
 * @brief Constructs a CommandlineReader object
 * @param parent The parent object
 */
CommandlineReader::CommandlineReader(QObject *parent) : QObject(parent)
{
}

/**
 * @brief Calls run() asynchronously
 */
void CommandlineReader::runAsync()
{
	auto future = QtConcurrent::run(this, &CommandlineReader::run);
}

/**
 * @brief Reads lines from stdin.
 *
 * This function returns, when the user inputs "/quit" into the terminal.
 */
void CommandlineReader::run()
{
	QTextStream stdInput(stdin);
	QString line;
	bool cancel = false;
	while (!cancel && stdInput.readLineInto(&line)) {
		// parse the command
		if (!line.startsWith("/")) {
			qInfo() << "Commands start with /";
		} else {
			auto parts = line.split(' ', QString::SkipEmptyParts).toStdList();
			if (parts.empty()) {
				qInfo() << "Please enter a nonempty command";
			} else {
				const auto command = parts.begin()->mid(1);
				parts.pop_front();
				if (command == "addbot") {
					int amount;
					if (!takeInt(amount, parts, "You can also specify the number of bots to add")) {
						amount = 1;
					}
					for (int i = 0; i < amount; ++i) {
						emit addBot();
					}
				} else if (command == "chat") {
					QString message;
					if (takeString(message, parts, "Chat message")) {
						emit chat(message);
					}
				} else if (command == "help") {
					qInfo() << "/start starts the game";
				} else if (command == "itemspawn") {
					int index;
					float prob;
					if (takeInt(index, parts, "Index") && takeFloat(prob, parts, "Probability")) {
						emit itemSpawn(index, prob);
					}
				} else if (command == "itemwait") {
					int min, max;
					if (takeInt(min, parts, "Min") && takeInt(max, parts, "Max")) {
						emit itemWait(min, max);
					}
				} else if (command == "listen") {
					int port;
					if (takeInt(port, parts, "Pass the port")) {
						emit listen(port);
					}
				} else if (command == "logicupdate") {
					int rate;
					if (takeInt(rate, parts, "Logic update rate")) {
						emit logicUpdate(rate);
					}
				} else if (command == "networkupdate") {
					int rate;
					if (takeInt(rate, parts, "Network update rate")) {
						emit networkUpdate(rate);
					}
				} else if (command == "quit") {
					cancel = true;
					emit quit();
				} else if (command == "remove") {
					int index;
					if (takeInt(index, parts, "index")) {
						emit remove(index);
					}
				} else if (command == "reset") {
					emit reset();
				} else if (command == "resize") {
					int width, height;
					if (takeInt(width, parts, "width") && takeInt(height, parts, "height")) {
						emit resize(QPoint(width, height));
					}
				} else if (command == "start") {
					emit start();
				} else if (command == "targetscore") {
					int score;
					if (takeInt(score, parts, "Score to reach")) {
						emit targetScore(score);
					}
				} else {
					qInfo() << "Unknown command. See /help for more info.";
				}
			}
		}
	}
}

/**
 * @brief Runs some generic checks on a list
 * @param l The list to check
 * @return Whether the list passed the test
 */
bool CommandlineReader::checkList(const std::list<QString>& l)
{
	return !l.empty() && l.cbegin()->size() > 0;
}

/**
 * @brief Returns the first parameter from the list as float
 * @param result The first parameter interpreted as float
 * @param l The parameter list
 * @param info An info text about the parameter
 * @return Whether the operation was successful
 */
bool CommandlineReader::takeFloat(float& result, std::list<QString>& l, QString info)
{
	bool ok = false;
	if (checkList(l)) {
		QString p = l.front();
		l.pop_front();
		result = p.toFloat(&ok);
	}
	if (!ok) {
		qInfo() << info;
	}
	return ok;
}

/**
 * @brief Returns the first parameter from the list as int
 * @param result The first parameter interpreted as int
 * @param l The parameter list
 * @param info An info text about the parameter
 * @return Whether the operation was successful
 */
bool CommandlineReader::takeInt(int& result, std::list<QString>& l, QString info)
{
	bool ok = false;
	if (checkList(l)) {
		QString p = l.front();
		l.pop_front();
		result = p.toInt(&ok);
	}
	if (!ok) {
		qInfo() << info;
	}
	return ok;
}

/**
 * @brief Returns the first parameter from the list as QString
 * @param result The first parameter interpreted as QString
 * @param l The parameter list
 * @param info An info text about the parameter
 * @return Whether the operation was successful
 */
bool CommandlineReader::takeString(QString& result, std::list<QString>& l, QString info)
{
	bool ok = false;
	if (checkList(l)) {
		result = l.front();
		l.pop_front();
		ok = !result.isEmpty();
	}
	if (!ok) {
		qInfo() << info;
	}
	return ok;
}
#endif // HEADLESS_SERVER
