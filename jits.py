__author__ = 'mcconnelldj'

import csv
import locale
import dutil
import sqlite3

# Declare these dictionaries so they have global scope
# Item count indexed by item name
countDict = {}
# Item total sales indexed by item name
salesDict = {}
# The next four dictionaries are for breaking down sales
# by category.  The item name is prefixed with the top-level
# category and separated with a colon.
alcoholDict = {}
foodDict = {}
wineDict = {}
beerDict = {}

locale.setlocale(locale.LC_ALL, '')
#dataConnector = sqlite3.connect('jits.db')
#dbcursor = dataConnector.cursor()
#dbcursor.execute('''CREATE TABLE alcohol
#                    (date text, name text, qty int)''')

with open('data/2015_06.csv') as f:
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
            if line[0] == 'Item Name':
                headerRow = True


    for row in reader:

        if len(row):

            if row[0] not in countDict and len(row) > 2:
                # Row index 0 is the ItemCategory:ItemName
                # Row index 1 is the number of items sold or voided
                # Sold or voided depends on the number of data items if
                # the row.  If there are 6 items, it's a sale.
                # If there are 4 items, it's a voide
                countDict[row[0]] = int(row[1])
                # Row index 2 is the sales total in dollars.
                saleString = row[2]
                # Convert the string, which can include '$' sign and commas
                # to a float value
                sales = locale.atof(saleString[1:])
                # Use the same key, i.e. row index 0 to add the sale
                # float value to the sales dictionary.
                salesDict[row[0]] = sales

            elif len(row) == 6:  # This item is a Sale

                # This item is already in the count dictionary, so it's
                # a duplicate.  Increment the count of items sold and
                # increase the total sales by the additional amount
                currentCount = countDict[row[0]]
                newCount = currentCount + int(row[1])
                countDict[row[0]] = newCount

                currentSales = salesDict[row[0]]
                saleString = row[2]
                newSales = locale.atof(saleString[1:])
                totalSales = currentSales + newSales
                avgSale = totalSales / newCount
                salesDict[row[0]] = totalSales

            elif len(row) == 4:  # This item is a Void

                # This item is a void in the report.  The item count
                # needs to be decremented by the number of voids
                currentCount = countDict[row[0]]
                newCount = currentCount - int(row[1])
                countDict[row[0]] = newCount

                # The sales amount also needs to be deducted from the
                # sales total
                currentSales = salesDict[row[0]]
                voidString = row[2]
                voidAmt = locale.atof(voidString[1:])
                newSales = currentSales - voidAmt
                salesDict[row[0]] = newSales


    # Sort items into their top-level category dictionaries
    for item in countDict:
        category = item.partition(': ')
        if ( category[0] == 'Alcohol'):
            alcoholDict[category[2]] = (countDict[item],salesDict[item])
        elif (category[0] == 'Food'):
            foodDict[category[2]] = (countDict[item],salesDict[item])
        elif (category[0] == 'Wine'):
            wineDict[category[2]] = (countDict[item],salesDict[item])
        elif (category[0] == 'Beer'):
            beerDict[category[2]] = (countDict[item],salesDict[item])

    print ('SALES FOR ALCOHOLIC BEVERAGES')
    for bev in alcoholDict:
        if alcoholDict[bev][0] == 0:
            avgp = 0
        else:
            avgp = '${:.2f}'.format(alcoholDict[bev][1]/alcoholDict[bev][0])
        print ( bev, ': sold', alcoholDict[bev][0], ', at average price of', avgp)

    print ('SALES FOR WINE')
    for glass in wineDict:
        if wineDict[glass][0] == 0:
            avgp = 0
        else:
            avgp = '${:.2f}'.format(wineDict[glass][1]/wineDict[glass][0])
        print ( glass, ': sold', wineDict[glass][0], ', at average price of', avgp)

    print ('SALES FOR BEER')
    for bottle in beerDict:
        if beerDict[bottle][0] == 0:
            avgp = 0
        else:
            avgp = '${:.2f}'.format(beerDict[bottle][1]/beerDict[bottle][0])
        print ( bottle, ': sold', beerDict[bottle][0], ', at average price of', avgp)

    print ('SALES FOR FOOD')
    for plate in foodDict:
        if foodDict[plate][0] == 0:
            avgp = 0
        else:
            avgp = '${:.2f}'.format(foodDict[plate][1]/foodDict[plate][0])
        print ( plate, ': sold', foodDict[plate][0], ', at average price of', avgp)

