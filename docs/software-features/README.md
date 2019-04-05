# Software features and programming tricks

## System groups
A set of system groups provides ability to categorize each item in system. I. e.:

- [Persistence](/src/conf/persistence/jdbc.persist) as a base for user defined groups
- [User system groups](/src/conf/items/common-groups.items)

Membership of the group provides ability to manipulate sets of items. I.e. simple rule to manage that only one HDMI entry can be active:

```java
rule "HDMI_Rule"
when
    Member of gHDMI received update
then
  if (triggeringItem.state.toString() == "ON"){
    gHDMI.members.forEach [E |
      if (triggeringItem.name != E.name) {
        postUpdate(E, OFF)
      }
    ]
  }
end
```

- Proper group and item naming

```java
String GF_FamilyRoom_Heartbeat "8266 Salon [%s]"  (GF_FamilyRoom, gHeartbeat) {mqtt="<[broker:myHome/GF_FamilyRoom_Heartbeat:state:JS(string.js):.*]"}
DateTime GF_FamilyRoom_Heartbeat_LastUpdate "8266 in family room [%1$td.%1$tm %1$tH:%1$tM]" <time> (gLastUpdate, gLastUpdateTimeout)
Switch GF_FamilyRoom_Heartbeat_LastUpdate_Error "8266 Family room" <time> (gErrors, gPersistenceSwitch)
```

`<item>`, `<item>_LastUpdate` and `<item>_LastUpdate_Error` attached to specific groups provide ability find proper item by name and locate other items necessery to manage more complex features.

- Feature oriented groups, i.e `gCalendar` has several switches (calclated in [rule](/src/conf/rules/common-calendar.rules)) to manage scenes.
