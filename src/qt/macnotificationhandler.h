#ifndef MACNOTIFICATIONHANDLER_H
#define MACNOTIFICATIONHANDLER_H
#include <QObject>

/** Macintosh-specific notification handler (supports UserNotificationCenter and Growl).
 */
class MacNotificationHandler : public QObject
{
    Q_OBJECT

public:   
    /** executes AppleScript */
    void sendAppleScript(const QString &script);

    static MacNotificationHandler *instance();
};


#endif // MACNOTIFICATIONHANDLER_H
