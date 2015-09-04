__author__ = 'mcconnelldj'

import csv
import locale
import datetime
import sqlite3

countDict = {}
salesDict = {}
locale.setlocale(locale.LC_ALL, '')
#dataConnector = sqlite3.connect('jits.db')
#dbcursor = dataConnector.cursor()
#dbcursor.execute('''CREATE TABLE alcohol
#                    (date text, name text, qty int)''')

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
    day = int(dsegs[1][:-1])
    year = int(dsegs[2][:-1])

    # Break time into hour, minute and seconds
    hms = dsegs[3].split(':')
    hour = int(hms[0])
    minute = int(hms[1])
    second = int(hms[2])

    #adjust for AM/PM
    if dsegs[4] == 'PM' and hour != 12:
        hour += 12
    if dsegs[4] == 'AM' and hour == 12:
        hour = 0

    return datetime.datetime(year,month,day,hour,minute,second)

with open('data/Report.csv') as f:
    startData = False
    reader = csv.reader(f)

    # Read first row, which contains date range information
    dateString = next(reader)
    tmp1 = dateString[0][6:]
    piece1 = tmp1.partition(' To: ')
    piece2 = piece1[2].partition('\nGenerated On: ')
    reportBegin = piece1[0]
    reportEnd = piece2[0]
    reportCreated = piece2[2]

    dtBegin  = str2datetime(reportBegin)
    dtEnd    = str2datetime(reportEnd)
    dtCreate = str2datetime(reportCreated)

    # Read second row, which is the time stamp of the report
    timeStamp = next(reader)

    # Continue reading rows until the header row for the data is found
    headerRow = False
    while not headerRow:
        line = next(reader)
        if len(line):
            if line[0] == 'Item Name':
                headerRow = True


    for row in reader:

        if len(row):

            if row[0] not in countDict and len(row) > 2:

                countDict[row[0]] = int(row[1])
                saleString = row[2]
                sales = locale.atof(saleString[1:])
                salesDict[row[0]] = sales
                avgSale = sales / int(row[1])
                # print 'SOLD:', int( row[1] ), row[0], 'at average', fpformat.fix( avgSale, 2 ), 'total', row[2]

            #else:

            elif len(row) == 6:  # This item is a Sale

                currentCount = countDict[row[0]]
                newCount = currentCount + int(row[1])
                countDict[row[0]] = newCount

                currentSales = salesDict[row[0]]
                saleString = row[2]
                newSales = locale.atof(saleString[1:])
                totalSales = currentSales + newSales
                avgSale = totalSales / newCount
                salesDict[row[0]] = totalSales

                # print 'UPDATE COUNT:', row[0], 'to', newCount, 'at', fpformat.fix( avgSale, 2 )
                #print 'UPDATE SALES:', row[0], 'to', currentSales + newSales, 'total'

            elif len(row) == 4:  # This item is a Void

                currentCount = countDict[row[0]]
                newCount = currentCount - int(row[1])
                countDict[row[0]] = newCount
                # print 'VOID: ',int( row[1] ), row[0], 'to', newCount


    for item in countDict:

        if countDict[item] > 0:
            avgSale = '${:.2f}'.format(salesDict[item] / countDict[item])
            total = '${:.2f}'.format(salesDict[item])

            print('SOLD', countDict[item], item, 'at', avgSale, 'each, total =', total)
