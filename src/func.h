#ifndef FUNC_H
#define FUNC_H
#include <QString>
#include <QDir>
#include <QComboBox>
#ifdef Q_OS_WIN
#include <stdint.h>
#endif

QString vCardDateFormat(int index = 0);
void setVCardDateFormat(QString format);

QString readFromList(QStringList *list,QString text);
void wakeOnLan(QString macAddress,QChar separator = '-');

QString appName(void);
QString findDocPath(QString binPath = "");
QString getDocPath(void);
QString findIniPath(QString binPath = "");
QString getIniPath(void);
QString findLogoPath(QString binPath = "");
QString getLogoPath(void);
QString getHelpFile(void);
QString getBaseFileName(void);
QString getIniFile(void);
QString getStyleSheetFileName(void);
QString getStyleSheetFile(void);
QString getTranslationFile(QString locale = "");
QString getQtTranslationFile(QString locale = "");


QString timeToString(int64_t time);
QString day(QDateTime(dateTime));
QString timeStr_hhmm(time_t time);
QString timeStr_ddhhmm(time_t time);
QString timeStr_ddddmm(time_t time);
QString timeStr_date(time_t time);
QString timeStr_time(time_t time);
time_t minimumTime(time_t t);

bool saveToFile(QString fileName,QString text,QString codecStr);
bool saveToFile(QString fileName,QString text);
QString loadFromFile(QString fileName);
QString loadFromFile(QString fileName,QString codec);
QFile::Permissions setFileRights(QString fileName,QFile::Permissions newPermission = 0);
/*
class Settings : public QObject {
    Q_OBJECT
public:
    Settings(QString fileName = "");
    //~Settings();
    void clear(void);
    void read(void);
    void write(void);
    void append(QString text);
    QStringList groups(QString beginWith = "");
    QStringList group(QString groupName);
    QString value(QString groupName, QString keyName);
    bool bValue(QString groupName, QString keyName);
    void setValue(QString groupName, QString keyName,QString value);
    void setValue(QString groupName, QString keyName,bool bValue);
    void insert(QString groupName, QString keyName,QString value);
    int indexOf(QString groupName, QString keyName);
    QString listSeparator(void);
    //void setKey(QString group, QString key, QString value);
    //void setList(QString group, QString key, QStringList value);
    //void setGroup(QString group, QString value);
signals:
    void keyRead(QString group, QString key, QString value);
    void keyRead(QString group, QString key, bool value);
    void listRead(QString group, QString key, QStringList value);
    void groupRead(QString group, QStringList value);
private:
    QString m_fileName;
    QStringList m_rawSettings;
    QString m_keySeparator;
    QString m_listSeparator;
    // QList<QStringList *>
};
*/
//##############################################################################################
void copyFiles(QString path, QDir dest, int basePathLength = -1);
bool createFile(QString fileName);
QString parentOf(QString path);
//QString findTrashPath(QString fileName /* filename to create a new file - with full Path */);
QString createTrashPath(QString fileName,QString trashName);
QString UtcDate();
QString Date();
int tagLength(QString str, int fromPos);
void setComboBoxIndex(QComboBox *box, QString text);
QString textToHtml4(QString text);
QString pathOfFile(QString fileName);
QString fileOfPath(QString fileName);
bool fileIsPath(QString fileName);


const char *textCodec(void);
void setTextCodec(QString codec);
bool copyTextFile(QString fileNameFrom,QString fileNameTo);

QString strRepeat(QString zeichen, int n=1);
QString stringFromList(QStringList list, QString trenn = "", int start = 0); 

QStringList xfilesOfDir(QString path);
#endif
