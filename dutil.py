__author__ = 'mcconnelldj'

import datetime

monthmap = {
    'Jan': 1,
    'Feb': 2,
    'Mar': 3,
    'Apr': 4,
    'May': 5,
    'Jun': 6,
    'Jul': 7,
    'Aug': 8,
    'Sep': 9,
    'Oct': 10,
    'Nov': 11,
    'Dec': 12}


def str2datetime(dtstr):
    month = 1
    # Takes a string from the POS report and creates
    # a datetime object
    # Typical string is: May 5, 2015, 2:52:49 PM

    # Break string into pieces based on whitespace
    dsegs = dtstr.split()

    # Lookup int value of month string
    mstr = dsegs[0]
    if mstr in monthmap:
        month = monthmap[mstr]
    else:
        return datetime.datetime()
    day = int(dsegs[1][:-1])
    year = int(dsegs[2][:-1])

    # Break time into hour, minute and seconds
    hms = dsegs[3].split(':')
    hour = int(hms[0])
    minute = int(hms[1])
    second = int(hms[2])

    # adjust for AM/PM
    if dsegs[4] == 'PM' and hour != 12:
        hour += 12
    if dsegs[4] == 'AM' and hour == 12:
        hour = 0

    return datetime.datetime(year, month, day, hour, minute, second)

