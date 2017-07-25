__author__ = 'mcconnelldj'

import csv
import locale
import dutil
import sys
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

def processMenu(csvFile):
    startEndDate = ""
    with open(csvFile) as f:

        reader = csv.reader(f)
        # read the first line
        line1 = next(reader)
        # Split the line so that the start-end date string can be grabbed
        pieces = line1[0].split('-',2)
        # Save the start-end string for use in naming the output files
        startEndDate = pieces[-1]
        print(startEndDate)

        # Discard the next line, which is the column headings
        next(reader)

        # Iterate over lines in the CSV file
        for line in reader:

            # The elements in the list 'line' are:
            # line[0] Item Name
            # line[1] Sales Category
            # line[2] Menu Category
            # line[3] Quantity
            # line[4] Sales
            # line[5] Voids

            # Skip over any remaining lines that have less than two cells.
            if len(line) >= 2:
                if line[1] == "Beer":
                    saleAmt = locale.atof(line[6].lstrip('-$'))
                    voidAmt = locale.atof(line[5].lstrip('-$'))
                    saleQty     = locale.atof(line[3])
                    voidQty     = locale.atof(line[4])
                    beerDict[line[0]] = (saleQty - voidQty, saleAmt - voidAmt)

                elif line[1] == "Wine":

                    saleAmt = locale.atof(line[6].lstrip('-$'))
                    voidAmt = locale.atof(line[5].lstrip('-$'))
                    saleQty     = locale.atof(line[3])
                    voidQty     = locale.atof(line[4])
                    wineDict[line[0]] = (saleQty - voidQty, saleAmt - voidAmt)

                elif line[1] == "Alcohol":
                    saleAmt = locale.atof(line[6].lstrip('-$'))
                    voidAmt = locale.atof(line[5].lstrip('-$'))
                    saleQty     = locale.atof(line[3])
                    voidQty     = locale.atof(line[4])
                    alcoholDict[line[0]] = (saleQty - voidQty, saleAmt - voidAmt)

                elif line[1] == "Food":
                    saleAmt = locale.atof(line[6].lstrip('-$'))
                    voidAmt = locale.atof(line[5].lstrip('-$'))
                    saleQty     = locale.atof(line[3])
                    voidQty     = locale.atof(line[4])
                    foodDict[line[0]] = (saleQty - voidQty, saleAmt - voidAmt)

#    print ('SALES FOR ALCOHOLIC BEVERAGES')
#    for bev in alcoholDict:
#        if alcoholDict[bev][0] == 0:
#            avgp = 0
#        else:
#            avgp = '${:.2f}'.format(alcoholDict[bev][1]/alcoholDict[bev][0])
#        print ( bev, ': sold', alcoholDict[bev][0], ', at average price of', avgp)

    # For Wine and Liquor sales, need to consolidate variants of the same
    # product into one total quantity, e.g. wine glass, bottle, and 9oz pour
    consolidatewine = {}
    for menuitem in wineDict:
        # make sure there's no trailing white space
        item = menuitem.rstrip()
        # determine the volume based on Glass, Bottle or 9oz
        volume = 0
        sales = 0
        root = ''
        if (item[-5:] == 'Glass'):
            root = item[:-6]
            volume = wineDict[menuitem][0] * 177.441
            sales = wineDict[menuitem][1]
        elif (item[-6:] == 'Bottle'):
            root = item[:-7]
            volume = wineDict[menuitem][0] * 750.
            sales = wineDict[menuitem][1]
        elif (item[-3:] == '9oz'):
            root = item[:-4]
            volume = wineDict[menuitem][0] * 266.162
            sales = wineDict[menuitem][1]
        else:
            root = item
            volume = wineDict[menuitem][0] * 750.
            sales = wineDict[menuitem][1]
        #

        if (root not in consolidatewine and root != ''):
            consolidatewine[root] = (volume, sales)
        elif (root != ''):
            currentVolume = consolidatewine[root][0]
            newVolume = currentVolume + volume
            currentSales = consolidatewine[root][1]
            newSales = currentSales + sales
            consolidatewine[root] = (newVolume, newSales)

    # Create file for wine sales and output data.
    nameWine = startEndDate + '_wineSales.csv'
    wineSales = open(nameWine,'w')
    wineSales.write('CONSOLIDATED WINE SALES\n')
    wineSales.write('Wine,Bottles Sold,Avg. Price per Bottle\n')
    for wine in consolidatewine:
        bottles = 0
        if consolidatewine[wine][0] == 0:
            avgp = 0
            pbottles = '0'
            outline = wine + ',0,0\n'
        else:
            bottles = consolidatewine[wine][0] / 750.
            pbottles = '{:.2f}'.format(bottles)
            avgp = '${:.2f}'.format(consolidatewine[wine][1] / bottles)
            outline = wine + ',' + pbottles + ',' + avgp + '\n'
        wineSales.write(outline)

    #    print ('SALES FOR BEER')
#    for bottle in beerDict:
#        if beerDict[bottle][0] == 0:
#            avgp = 0
#        else:
#            avgp = '${:.2f}'.format(beerDict[bottle][1]/beerDict[bottle][0])
#        print ( bottle, ': sold', beerDict[bottle][0], ', at average price of', avgp)

#    print ('SALES FOR FOOD')
#    for plate in foodDict:
#        if foodDict[plate][0] == 0:
#            avgp = 0
#        else:
#            avgp = '${:.2f}'.format(foodDict[plate][1]/foodDict[plate][0])
#        print ( plate, ': sold', foodDict[plate][0], ', at average price of', avgp)

    return 0

##### _Main_

lineInput = sys.argv
inputFile = lineInput[1]
if len(inputFile) > 0:
    processMenu(inputFile)
else:
    processMenu('data/infile.csv')