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
cocktailDict = {}
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
                    cleanName = line[0].rstrip()
                    if cleanName.startswith('$5 '):
                        baseName = cleanName[3:]
                    else:
                        baseName = cleanName
                    saleAmt = locale.atof(line[6].lstrip('-$'))
                    voidAmt = locale.atof(line[5].lstrip('-$'))
                    saleQty     = locale.atof(line[3])
                    voidQty     = locale.atof(line[4])
                    if baseName == 'Raspberry Limoncello Prosecco':
                        cocktailDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)
                    elif baseName == 'Al Capone':
                        cocktailDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)
                    elif baseName == 'The Little Italy':
                        cocktailDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)
                    elif baseName == 'Lucky Luciano':
                        cocktailDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)
                    elif baseName == 'Southern Italian Tea':
                        cocktailDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)
                    elif baseName == 'Long Insland':
                        cocktailDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)
                    elif baseName == 'Bellini':
                        cocktailDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)
                    elif baseName == 'Tawny Port':
                        cocktailDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)
                    elif baseName == 'Cafe Toledo':
                        cocktailDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)
                    elif baseName == 'Country Boy Mimosa':
                        cocktailDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)
                    elif baseName == 'Blberry JD Collins':
                        cocktailDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)
                    elif baseName == 'Hendricks Mule':
                        cocktailDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)
                    elif baseName == 'Italian Margarita':
                        cocktailDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)
                    elif baseName == 'Apple Bourbon Cool Toddy':
                        cocktailDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)
                    elif baseName == 'Bloody Mary':
                        cocktailDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)
                    elif baseName == 'Irish Coffee':
                        cocktailDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)
                    elif baseName == 'Mimosa':
                        cocktailDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)
                    else:
                        alcoholDict[baseName] = (saleQty - voidQty, saleAmt - voidAmt)

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

    # Consolidate wine variants of the same product into
    # one total quantity, e.g. wine glass, bottle, and 9oz pour
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
            avgp = '{:.2f}'.format(consolidatewine[wine][1] / bottles)
            outline = wine + ',' + pbottles + ',' + avgp + '\n'
        wineSales.write(outline)

    # Consolidate liquor variants of the same product into
    # one total quantity, e.g. regular (no suffix), 2oz, Up pour
    consolidateliquor = {}
    # Need a separate dictionary for named cocktails
    for menuitem in alcoholDict:
        # make sure there's no trailing white space
        item = menuitem.rstrip()
        # determine the volume based on regular shot, 2oz or Up
        volume = 0
        sales = 0
        root = ''
        if (item[-4:] == ' 2oz'):
            root = item[:-4]
            volume = alcoholDict[menuitem][0] * 59.1471
            sales = alcoholDict[menuitem][1]
        elif (item[-3:] == ' Up'):
            root = item[:-3]
            volume = alcoholDict[menuitem][0] * 88.7206
            sales = alcoholDict[menuitem][1]
        else:
            root = item
            if root == 'Martini':
                root = 'Vodka'
                volume = alcoholDict[menuitem][0] * 88.7206
                sales = alcoholDict[menuitem][1]
            elif root == 'Bloody Mary':
                root = 'Vodka'
                volume = alcoholDict[menuitem][0] * 59.1471
                sales = alcoholDict[menuitem][1]
            elif root[-9:] == 'Margarita':
                root = 'Tequila'
                volume = alcoholDict[menuitem][0] * 36.96691
                sales = alcoholDict[menuitem][1]
            elif root == 'Hendricks Mule':
                root = 'Hendricks Gin'
                volume = alcoholDict[menuitem][0] * 59.1471
                sales = alcoholDict[menuitem][1]
            elif root == 'Apple Bourbon Cool Toddy':
                root = 'Coopers Craft'
                volume = alcoholDict[menuitem][0] * 59.1471
                sales = alcoholDict[menuitem][1]
            elif root == 'Irish Coffee':
                root = 'Bushmills'
                volume = alcoholDict[menuitem][0] * 36.96691
                sales = alcoholDict[menuitem][1]
            else:
                volume = alcoholDict[menuitem][0] * 36.96691
                sales = alcoholDict[menuitem][1]

        if (root not in consolidateliquor and root != ''):
            consolidateliquor[root] = (volume, sales)
        elif (root != ''):
            currentVolume = consolidateliquor[root][0]
            newVolume = currentVolume + volume
            currentSales = consolidateliquor[root][1]
            newSales = currentSales + sales
            consolidateliquor[root] = (newVolume, newSales)

    # Create file for liquor sales and output data.
    nameLiquor = startEndDate + '_liquorSales.csv'
    liquorSales = open(nameLiquor,'w')
    liquorSales.write('CONSOLIDATED LIQUOR SALES,' + startEndDate + '\n')
    liquorSales.write('Liquor, Liters, Shots, Avg price/shot\n')
    for liquor in consolidateliquor:
        ml = 0
        if consolidateliquor[liquor][0] == 0:
            avgp = 0
            pml = '0'
            pshots = '0'
            outline = liquor + ',0,0,0\n'
        else:
            ml = consolidateliquor[liquor][0]
            shots = ml/36.96691
            pml = '{:.2f}'.format(ml)
            pshots = '{:.2f}'.format(shots)
            avgp = '{:.2f}'.format(consolidateliquor[liquor][1] / shots)
            outline = liquor + ',' + pml + ',' + pshots + ','+ avgp + '\n'
        liquorSales.write(outline)

    # Write out cocktail sales to the same file for liquor sales
    liquorSales.write('\nCOCKTAIL SALES,' + startEndDate + '\n')
    liquorSales.write('Cocktail, Sold, Avg price\n')
    for cocktail in cocktailDict:
        if cocktailDict[cocktail][0] == 0:
            avgp = 0
            pcount = '0'
            outline = cocktail + ',0,0\n'
        else:
            count = cocktailDict[cocktail][0]
            pcount = '{:.2f}'.format(count)
            avgp = '{:.2f}'.format(cocktailDict[cocktail][1] / count)
            outline = cocktail + ',' + pcount + ',' + avgp + '\n'
        liquorSales.write(outline)


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
