#include "macnotificationhandler.h"

#undef slots
#include <Cocoa/Cocoa.h>

// sendAppleScript just take a QString and executes it as apple script
void MacNotificationHandler::sendAppleScript(const QString &script)
{
    QByteArray utf8 = script.toUtf8();
    char* cString = (char *)utf8.constData();
    NSString *scriptApple = [[NSString alloc] initWithUTF8String:cString];

    NSAppleScript *as = [[NSAppleScript alloc] initWithSource:scriptApple];
    NSDictionary *err = nil;
    [as executeAndReturnError:&err];
    [as release];
    [scriptApple release];
}

MacNotificationHandler *MacNotificationHandler::instance()
{
    static MacNotificationHandler *s_instance = NULL;
    if (!s_instance)
        s_instance = new MacNotificationHandler();
    return s_instance;
}
