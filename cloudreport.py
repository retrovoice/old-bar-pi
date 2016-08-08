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
wineglassDict = {}
winebottleDict = {}
beerDict = {}

locale.setlocale(locale.LC_ALL, '')

def processMenu(csvFile):
    with open(csvFile) as f:

        reader = csv.reader(f)

        # Iterate over lines in the CSV file
        for line in reader:

            # The elements in the list 'line' are:
            # line[0] Item Name
            # line[1] Sales Category
            # line[2] Menu Category
            # line[3] Quantity
            # line[4] Sales
            # line[5] Voids

            if line[1] == "Beer":
                saleAmt = locale.atof(line[4])
                voidAmt = locale.atof(line[5])
                qty     = locale.atof(line[3])
                beerDict[line[0]] = (qty, saleAmt - voidAmt)

            elif line[1] == "Wine":

                bSuffix = line[0][-7:]
                gSuffix = line[0][-6:]

                # look for bottle suffix
                if bSuffix.lower() == ' bottle':
                    wkey = line[0][:-7]

                # look for glass suffix
                elif gSuffix.lower() == ' glass':
                    wkey = line[0][:-6]

                # no bottle or glass suffix to remove
                else:
                    wkey = line[0]

                saleAmt = locale.atof(line[4])
                voidAmt = locale.atof(line[5])
                qty     = locale.atof(line[3])
                if line[2][-5:] == "Glass" and line[0][0:11] != "Villa Sandi":
                    wineglassDict[wkey] = (qty, saleAmt - voidAmt)
                else:
                    winebottleDict[wkey] = (qty, saleAmt - voidAmt)

            elif line[1] == "Alcohol":
                saleAmt = locale.atof(line[4])
                voidAmt = locale.atof(line[5])
                qty     = locale.atof(line[3])
                alcoholDict[line[0]] = (qty, saleAmt - voidAmt)

            elif line[1] == "Food":
                saleAmt = locale.atof(line[4])
                voidAmt = locale.atof(line[5])
                qty     = locale.atof(line[3])
                foodDict[line[0]] = (qty, saleAmt - voidAmt)

    # Roll wine glass sales into bottle totals

    for glass in wineglassDict:
        if glass in winebottleDict:
            print("added ", glass, " to bottle")
            btlAmt = wineglassDict[glass][0]/4.0
            glassSales = wineglassDict[glass][1]
            current = winebottleDict[glass]
            updated = (current[0]+btlAmt,current[1] + glassSales)
            winebottleDict[glass] = updated
        else:
            winebottleDict[glass] = (wineglassDict[glass][0]/4.0, wineglassDict[glass][1])

    # Count Prosecco used in cocktail
    if 'Raspberry Limoncello Prosecco' in alcoholDict:
        proseccoBtls = alcoholDict['Raspberry Limoncello Prosecco'][0]/4.0
        proseccoSales = alcoholDict['Raspberry Limoncello Prosecco'][1]
        if 'Nino Franco Prosecco' in winebottleDict:
            currentCount = winebottleDict['Nino Franco Prosecco'][0]
            currentSales = winebottleDict['Nino Franco Prosecco'][1]
            update = (currentCount + proseccoBtls, currentSales + proseccoSales)
            winebottleDict['Nino Franco Prosecco'] = update
        else:
            winebottleDict['Nino Franco Prosecco'] = (proseccoBtls, proseccoSales)

    # Count Champagne used in Mimosas
    if 'Mimosa' in alcoholDict:
        champBtls = alcoholDict['Mimosa'][0]/4.0
        champSales = alcoholDict['Mimosa'][1]
        if 'Charles De Fere' in winebottleDict:
            currentCount = winebottleDict['Charles De Fere'][0]
            currentSales = winebottleDict['Charles De Fere'][1]
            update = (currentCount + champBtls, currentSales + champSales)
            winebottleDict['Charles De Fere'] = update
        else:
            winebottleDict['Charles De Fere'] = (champBtls, champSales)

#    print ('SALES FOR ALCOHOLIC BEVERAGES')
#    for bev in alcoholDict:
#        if alcoholDict[bev][0] == 0:
#            avgp = 0
#        else:
#            avgp = '${:.2f}'.format(alcoholDict[bev][1]/alcoholDict[bev][0])
#        print ( bev, ': sold', alcoholDict[bev][0], ', at average price of', avgp)

    print ('SALES FOR WINE')
    for bottle in winebottleDict:
        if winebottleDict[bottle][0] == 0:
            avgp = 0
        else:
            avgp = '${:.2f}'.format(winebottleDict[bottle][1]/winebottleDict[bottle][0])
        print ( bottle, ': sold', winebottleDict[bottle][0], ', at average price of', avgp)

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

processMenu('data/20160808.csv')
