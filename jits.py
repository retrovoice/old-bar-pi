__author__ = 'mcconnelldj'

import csv
import locale

countDict = {}
salesDict = {}
locale.setlocale(locale.LC_ALL, '')

with open('data/Report.csv') as f:
    startData = False
    reader = csv.reader(f)

    for row in reader:

        if startData:

            if row[0] not in countDict and len(row):

                countDict[row[0]] = int(row[1])
                saleString = row[2]
                sales = locale.atof(saleString[1:])
                salesDict[row[0]] = sales
                avgSale = sales / int(row[1])
                # print 'SOLD:', int( row[1] ), row[0], 'at average', fpformat.fix( avgSale, 2 ), 'total', row[2]

            else:

                if len(row) == 6:  # This item is a Sale

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

        if len(row):
            if row[0] == 'Item Name':
                startData = True

    for item in countDict:

        if countDict[item] > 0:
            avgSale = '${:.2f}'.format(salesDict[item] / countDict[item])
            total = '${:.2f}'.format(salesDict[item])

            print('SOLD', countDict[item], item, 'at', avgSale, 'each, total =', total)