
#include "func.h"
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QTextStream>
#include <QMessageBox>
//#include <QUdpSocket>
#include <QTextCodec>
#include <ctime>

QString _textCodec = "UTF-8";
QByteArray *buffer;


QString docPath = "";
QString iniPath = "";
QString logoPath = "";
QString applicationsName = "QTApp";

QStringList VCardDateFormat(QStringList() << "yyyyMMdd");

QString vCardDateFormat(int index) {
    if ( index < -1) return "";
    if ( index >= VCardDateFormat.count() ) return "";
    if ( index == -1) return VCardDateFormat.join(";");
    return  VCardDateFormat[index];
}
void setVCardDateFormat(QString format) {
    VCardDateFormat = format.split(";");
}

/*
Unter Linux wird das Programm üblicherweise nach /usr/bin bzw.  /usr/local/bin installiert.
Die Hilfedateien und Übersetzungsdateien liegen dann unter /usr/share/QMediaOrg bzw. /usr/local/share/QMediaOrg 
Deshalb wird hier (relativ zum Programmordner) der Doc_Path auf ../share/QMediaOrg gesetzt. 
Sollte dieser Ordner dann nicht vorhanden sein, wird wie unter Windows ein untergeordneter Ordner namens 
./help angenommen 
*/
/*
Unter Linux wird das Programm üblicherweise nach /usr/bin bzw.  /usr/local/bin installiert.
Die vom Programm änderbaren Ini und sonstigen Dateien liegen dann unter /home/user/...
Für eine "Einfachinstallation" ziehe ich es aber vor alles innerhalb eines Ordners zu
Organisieren dann landet die Ini - Datei im selben Ordner wie das Programm selbst,
so wie das auch bei einer Windows Installation gemacht wird.
*/
QString readFromList(QStringList *list,QString text) {
    int n = text.size();
    for ( int i = 0; i < list->count(); i++) {
        if ( list->at(i).left(n) == text ) {
            return list->at(i).mid(n).trimmed();
        }
    }
    return "";
}
void wakeOnLan(QString /*macAddress*/,QChar /*separator*/){
//    QUdpSocket *udpSocket = new QUdpSocket();
//    QStringList macParts = macAddress.split(separator);
//    if ( macParts.count() != 6) {
//        // error
//        return;
//    }
//    QByteArray macValues;
//    for (int i = 0; i < 6; i++) {
//        macValues.append(macParts[i]);
//    }
//    QByteArray magicPacket = QByteArray::fromHex("ffffffffffff");

//    for (int i = 0; i < 16; i++) {
//        magicPacket.append(QByteArray::fromHex(macValues));
//    }
//    udpSocket->writeDatagram(magicPacket.data(), magicPacket.size(), QHostAddress::Broadcast, 40000);
//    delete udpSocket;
}

QString appName(void) {
    return applicationsName;
}

QString findDocPath(QString binPath){
    // 26-11-2011 Allgemein gültige Version
    applicationsName = QFileInfo(qApp->applicationFilePath()).baseName();
    if (binPath.isEmpty()) binPath = qApp->applicationDirPath();
    QString path;
#ifdef __linux__
    path = binPath.left(binPath.lastIndexOf(QDir::separator()))+"/share/"+applicationsName;
    QDir helpPath(path);
    if (helpPath.exists() ) {
        docPath = path+"/";
        logoPath = docPath+"logos/";
        iniPath = QDir::homePath()+"/EasyApps/"+applicationsName+"/";
        helpPath.setPath(iniPath);
        if (!helpPath.exists()) {
            helpPath.mkpath(iniPath);
        }
        QString styleName = applicationsName+".style";
        if (!QFile::exists(iniPath+styleName)) {
            QString styleSheet = loadFromFile(path+"/"+styleName);
            saveToFile(iniPath+styleName,styleSheet);
        }
        return docPath;
    }
#endif
    iniPath = binPath+"/";
    docPath = binPath+"/help/";
    logoPath = binPath+"/logos/";
    return docPath;
}
QString getDocPath(void){
	if ( docPath.isEmpty() ) {
		findDocPath(qApp->applicationDirPath());
	}
	return docPath;
}
QString findIniPath(QString binPath){
    findDocPath(binPath);
    return iniPath;
}
QString findLogoPath(QString binPath){
    findDocPath(binPath);
    return logoPath;
}
QString getIniPath(void){
        if ( iniPath.isEmpty() ) {
                findIniPath(qApp->applicationDirPath());
        }
        return iniPath;
}
QString getLogoPath(void){
        if ( logoPath.isEmpty() ) {
                findLogoPath(qApp->applicationDirPath());
        }
        return logoPath;
}
QString getBaseFileName(void){
    return getIniPath()+applicationsName;
}
QString getIniFile(void){
    return getIniPath()+applicationsName+".conf";
}
QString getHelpFile(void){
    return getDocPath()+applicationsName+".html";
}
QString getStyleSheetFileName(void){
    QString path = getIniPath();
    if ( path.indexOf("EasyApps") < 0) {
        path = getDocPath();
    }
    return path+applicationsName+".css";
}
QString getStyleSheetFile(void){
    QString userStyleFileName = getStyleSheetFileName();
    QString style = loadFromFile(userStyleFileName);
    if ( style.isEmpty() ) {
        QString defaultStyleFileName = getDocPath()+"/"+appName()+".css";
        style = loadFromFile(defaultStyleFileName);
        saveToFile(userStyleFileName,style);
    }
    return style;
}

QString getTranslationFile(QString locale){
    if ( locale.isEmpty() ) {
        locale = QLocale::system().name();
    }
    return getDocPath()+applicationsName+"_"+locale;
}
QString getQtTranslationFile(QString locale){
    if ( locale.isEmpty() ) {
        locale = QLocale::system().name();
    }
    return getDocPath()+"qt_"+locale;
}
QString loadFromFile(QString fileName,QString codecStr){
    QString text="";
    QFile data(fileName);
    QTextStream in(&data);
    in.setAutoDetectUnicode(false);
//    in.setCodec("ISO 8859-1");
    in.setCodec(codecStr.toLocal8Bit().data());
    if ( data.open(QFile::ReadOnly ) ) {
        text = in.readAll();
    }
    data.close();
    return text;
}
QString loadFromFile(QString fileName){
    QString text="";
    QFile data(fileName);
    QTextStream in(&data);
    in.setAutoDetectUnicode(false);
    in.setCodec(textCodec());
    if ( data.open(QFile::ReadOnly ) ) {
        text = in.readAll();
    }
    data.close();
    return text;
}

bool saveToFile(QString fileName,QString text,QString codecStr){
    QString oldText = loadFromFile(fileName);
    if ( oldText == text ) {
        return true;
    }
    bool result = false;
    QFile data(fileName);
    QTextStream out(&data);
    out.setCodec(codecStr.toLocal8Bit().data());
    if ( data.open(QFile::WriteOnly | QFile::Truncate ) ) {
        out << text.trimmed();
        data.flush();
        result = true;
    }
    data.close();
    return result;
}
bool saveToFile(QString fileName,QString text){
    QString oldText = loadFromFile(fileName);
    if ( oldText == text ) {
        return true;
    }
    bool result = false;
    QFile data(fileName);
    QTextStream out(&data);
    out.setCodec(textCodec());
    if ( data.open(QFile::WriteOnly | QFile::Truncate ) ) {
        out << text.trimmed();
        data.flush();
        result = true;
    }
    data.close();
    return result;
}
QFile::Permissions setFileRights(QString fileName,QFile::Permissions newPermission) {
    QFile::Permissions permission = 0;
    //QFile file(fileName);
    if ( QFile::exists(fileName) ) {
        permission = QFile::permissions(fileName);
        QFile::setPermissions(fileName,newPermission);
//        QFile::Permissions testPermission = QFile::permissions(fileName);
//        if (testPermission != newPermission) {
//            QMessageBox msgBox;
//            msgBox.setText(QObject::tr("Set the script")+" \""+fileName+"\" "+QObject::tr("to executable"));
//            msgBox.exec();
//        }
    }
    return permission;
}
QString day(QDateTime dateTime) {
    int day = dateTime.date().dayOfWeek();
    QLocale l;
    return l.dayName(day,QLocale::ShortFormat);
}
QString timeToString(int64_t time) {
    int s = time / 1000;
    int h = (int) (s / 3600);
    s -= (h*3600);
    int m = (int) (s / 60);
    s -= (m * 60);
    QString str;
    return str.sprintf("%02d:%02d:%02d",h,m,s);
}

QString timeStr_hhmm(time_t utcTime){
    QDateTime dateTime;
    dateTime.setTime_t(utcTime);
    QTime time = dateTime.time();
    return time.toString("hh:mm");
}
QString timeStr_ddddmm(time_t utcTime){
    QDateTime dateTime;
    dateTime.setTime_t(utcTime);
    return day(dateTime)+" "+dateTime.toString("dd.MM");
}
QString timeStr_ddhhmm(time_t utcTime){
    QDateTime dateTime;
    dateTime.setTime_t(utcTime);
    return day(dateTime)+"\n"+dateTime.toString("hh:mm");
}
QString timeStr_date(time_t utcTime){
    QDateTime dateTime;
    dateTime.setTime_t(utcTime);
    return dateTime.toString("yyyy-MM-dd");
}
QString timeStr_time(time_t utcTime){
    QDateTime dateTime;
    dateTime.setTime_t(utcTime);
    QTime time = dateTime.time();
    return time.toString("hhmm");
}
time_t minimumTime(time_t t) {
    // Zeit nicht kleiner als die aktuelle Zeit
    if ( t < time(0) ) {
        return time(0);
    }
    return t;
}

//###################################################################################
// Ende: neues Programmdesign 11-2011
//###################################################################################
void copyFiles(QString path, QDir dest, int basePathLength) {
	if ( basePathLength == -1 ) {
		basePathLength = path.size();
	}
	QDir dir(path);
	dir.setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
	dir.setSorting(QDir::DirsLast | QDir::Name);
	dir.setNameFilters(QStringList() << "*.*");
	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		QString fileName = fileInfo.filePath().mid(basePathLength);
		if ( fileInfo.isDir() ) {
			dest.mkpath(dest.path()+fileName);
			copyFiles(fileInfo.filePath(),dest,basePathLength);
		} else {
			QFile::copy(fileInfo.filePath(),dest.path()+fileName);
		}
	}
}

bool createFile(QString fileName){
	QFile data(fileName);
	QTextStream out(&data);
	if ( data.open(QFile::WriteOnly | QFile::Truncate ) ) {
		data.flush();
		data.close();
		return true;
	}
	data.close();
	return false;
}
QString parentOf(QString path){
	int i = path.lastIndexOf('/');
	if ( i ) {
		return path.left(i);
	}
	i = path.indexOf('/');
	return path.left(i+1);
}
QString tryToMoveFileToParent(QString path,QString &name) {
	QString newPath = parentOf(path);
	if ( QFile::rename(path+"/"+name,newPath+"/"+name) ) {
		return tryToMoveFileToParent(newPath,name);
	}
	QFile::remove(path+"/"+name);
	return path;
}
QString findTrashPath(QString newFileName /* filename to create a new file - with full Path */) {
	QFileInfo file(newFileName);
	file.setFile(file.path()+"/dummy.xxx");
	file.makeAbsolute();
	QString fileName = file.filePath();
	QDir dir(file.path());
	QString path(dir.canonicalPath()); // symlinks berücksichtigen
	if ( (path.at(1) == ':') | (path.at(1) == '/') ) {  // System mit z.B. 'C:\xxxx oder //net/xxx 
		return path.left(2)+"/";
	} else {
		if ( createFile(fileName) ) {

			QString name = file.fileName();
			return tryToMoveFileToParent(path,name)+"/";
		}
	}
	return "";
}
QString createTrashPath(QString fileName,QString trashName) {
// suche und evtl. erzeuge einen Ordner als "Papierkorb" der auf jedem Datenträger nur einmal vorkommen sollte
// bei Unix/Linux o.Ä. wird ja alles in einer Ordnerstruktur unter / angelegt und eine Unterscheidung
// nach verschiedenen Laufwerken nicht sofort möglich. Deshalb wird versucht eine Datei durch umbenennen 
// in den möglichen Ordner zu verschieben.
	QString trashPath = findTrashPath(fileName)+trashName;
	QDir dir(trashPath);
	if ( !dir.exists(trashPath) ) {
		dir.mkpath(trashPath);
	}
	return trashPath;
}
QString UtcDate() {
	QDateTime dt = QDateTime::currentDateTime();
	return dt.toUTC().toString("yyyy-MM-ddThh:mm:ss+0100");
}

QString Date() {
	QDateTime dt = QDateTime::currentDateTime();
	return dt.toString("dd.MM.yyyy - hh:mm:ss");
	//return dt.toString();
}

void setComboBoxIndex(QComboBox *box, QString text){
	for ( int i=0; i< box->count() ; i++) {
		if ( box->itemText(i) == text ) {
			box->setCurrentIndex(i);
			return;
		}
	}
}
QString textToHtml4(QString text){
	QString result = "";
	ushort value;
	for ( int i = 0; i < text.size(); i++) {
		QChar z = text[i];
		value = z.unicode();
		if ((value > 0) & (value < 128)) {
			result += text[i];
		} else {
			result += "&#";
			result += QString::number(value);
		}
	}
	return result;
}

QString pathOfFile(QString fileName){
	int pos = fileName.lastIndexOf('/');
	if ( pos > -1 ) {
		return fileName.left(pos);
	} else {
		return fileName;
	}
}
QString fileOfPath(QString fileName){
	int pos = fileName.lastIndexOf('/');
	if ( pos > -1 ) {
		return fileName.mid(pos+1);
	} else {
		return fileName;
	}
}
bool fileIsPath(QString fileName){
	// check for double "myname" as "dir and file" like in .../somedir/myname/myname.*
	int point = fileName.lastIndexOf(".");
	int slash = fileName.lastIndexOf("/");
	QString file = fileName.mid(slash,point-slash);
	QString path = fileName.mid(slash-file.size(),file.size());
	return ( path == file );
}


void setTextCodec(QString codec){
	_textCodec = codec;
}

const char *textCodec(void){
	if ( buffer ) {
		delete buffer;
	}
    buffer = new QByteArray(_textCodec.toLatin1());
	return buffer->constData();
}

bool copyTextFile(QString fileNameFrom,QString fileNameTo){
	return saveToFile(fileNameTo,loadFromFile(fileNameFrom));
}


QString strRepeat(QString zeichen, int n ){
	QString result = "";
	for (int i = 0; i < n; i++)
		result += zeichen;
	return result;	
}
QString stringFromList(QStringList list, QString trenn, int start ){
	QString result;
	for (int i = start; i < list.count(); i++) {
		if (i != start) result += trenn;
		result += list[i]; 
	}
	return result;
}

QStringList xfilesOfDir(QString path){
	QDir dir(path);
	dir.setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
	dir.setSorting(QDir::DirsLast | QDir::Name);
	dir.setNameFilters(QStringList() << "*.*");
	QStringList listFiles;
	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		QString fileName = fileInfo.filePath();
		if ( fileInfo.isDir() ) {
			listFiles << "DIR:"+fileName;
			listFiles << xfilesOfDir(fileInfo.filePath());
		} else {
			listFiles << fileName;
		}
	}
	return listFiles;
}
