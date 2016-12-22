__author__ = 'mcconnelldj'

import csv
import locale
import dutil
# This dictionary is for modifiers
modDict = {}

locale.setlocale(locale.LC_ALL, '')


def processModifiers(modFile):
    with open(modFile) as f:
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

        dtBegin  = dutil.str2datetime(reportBegin)
        dtEnd    = dutil.str2datetime(reportEnd)
        dtCreate = dutil.str2datetime(reportCreated)

        # Continue reading rows until the header row for the data is found
        headerRow = False
        while not headerRow:
            line = next(reader)
            if len(line):
                if line[0] == 'Name':
                    headerRow = True

        for row in reader:

            if len(row):

                if row[0] not in modDict and len(row) > 2:
                    # Row index 0 is the Modifier Name
                    # Row index 1 is the Modifier Count
                    # Row index 2 is the Total Sales
                    saleString = row[2]
                    # Convert the string, which can include '$' sign and commas
                    # to a float value
                    sales = locale.atof(saleString.lstrip('-$'))
                    count = int(row[1])
                    # Create a tuple for the Count and Sales
                    x = (count,sales)
                    modDict[row[0]] = x

                else:

                    # This item is already in the dictionary, so it's
                    # a duplicate.  Increment the count of items sold and
                    # increase the total sales by the additional amount
                    print('<------>Duplicate modifier: ',row[0])
                    currentCount = modDict[row[0]][0]
                    newCount = currentCount + int(row[1])
                    currentSales = modDict[row[0]][1]
                    saleString = row[2]
                    newSales = locale.atof(saleString.lstrip('-$'))
                    totalSales = currentSales + newSales
                    modDict[row[0]] = (newCount, totalSales)

    print ('MODIFIERS REPORT')
    for m in modDict:
        if modDict[m][1] == 0:
            avgp = 0
        else:
            avgp = '${:.2f}'.format(modDict[m][1]/modDict[m][0])
        print ( modDict[m][0], ' instances of', m, ', at average price of', avgp)

    return 0

processModifiers('data/2016-07-modifiers.csv')