__author__ = 'mcconnelldj'

import csv
import locale
import dutil
import sys
import sqlite3

# These dictionaries are for breaking down sales
# by category.  Alcohol sales has an additional
# dictionary, cocktailDict, for tracking specific
# cocktail sales.
alcoholDict = {}
cocktailDict = {}
beerDict = {}
foodDict = {}
whiteWineDict = {}
redWineDict = {}

# This is a master dictionary for reading in Toast Item data.
masterDict = {}

locale.setlocale(locale.LC_ALL, '')

def processMenu(csvFile):
    with open(csvFile) as f:

        reader = csv.reader(f)
        # Read and discard the first line, which is the column headings
        next(reader)

        # Iterate over the remaining lines in the CSV file
        for line in reader:


            # The elements of interest in the list 'line' are:
            # line[0] Master ID
            # line[1] Item ID           ->[0] in dictionary
            # line[2] Parent ID         ->[1]
            # line[3] Menu Name         ->[2]
            # line[4] Menu Group        ->[3]
            # line[5] Subgroup          ->[4]
            # line[6] Menu Item         ->[5]
            # line[7] Avg Price         ->[6]
            # line[12] Item Qty         ->[7]
            # line[13] Gross Amount     ->[8]
            # line[14] Void Qty         ->[9]
            # line[15] Void Amount      ->[10]
            # line[16] Discount Amount  ->[11]
            # line[17] Net Amount       ->[12]

            if line[0]:
                # Convert Master ID text value to integer for use as dictionary key
                # Use only the characters from index 10 to the end, as the numbers
                # seem to all have 40000000000 as a prefix
                shortID = line[0].lstrip('40')
                #shortID = longID
                int_key = locale.atoi(shortID)

                # This is a new key, so simply add it to the dictionary
                if int_key not in masterDict and int_key > 0:
                    masterDict[int_key] = [line[1],line[2],line[3],line[4],line[5],
                                           line[6],locale.atof(line[7]),locale.atof(line[12]),
                                           locale.atof(line[13]),locale.atof(line[14]),
                                           locale.atof(line[15]),locale.atof(line[16]),locale.atof(line[17])]


                # There are some keys for open item buttons we don't care about
                elif int_key > 0:
                    # This key is already in dictionary, so update the entry by adding
                    # this item's Qty/Amount to previous total.

                    # print ("Duplicate key:  " + line[0] + ", for menu item " + masterDict[int_key][5])
                    # Note indexes in dictionary different than CVS file line
                    updatedLine = [masterDict[int_key][0],
                                   masterDict[int_key][1],
                                   masterDict[int_key][2],
                                   masterDict[int_key][3],
                                   masterDict[int_key][4],
                                   masterDict[int_key][5],
                                   masterDict[int_key][6],
                                   masterDict[int_key][7] + locale.atof(line[12]),
                                   masterDict[int_key][8] + locale.atof(line[13]),
                                   masterDict[int_key][9] + locale.atof(line[14]),
                                   masterDict[int_key][10] + locale.atof(line[15]),
                                   masterDict[int_key][11] + locale.atof(line[16]),
                                   masterDict[int_key][12] + locale.atof(line[17])]
                    masterDict[int_key] = updatedLine
    #print(masterDict)
    return 0


def consolidateWine():

    # This routine consolidates the wine by the bottle and by the
    # glass sales into a single quantity.  It uses a dictionary
    # with the name of the wine as the key.

    #print(masterDict["400000000005280263"][8] + ", " + masterDict["400000000005280373"][8])

    whiteWineDict["Promesso"] = masterDict[5280263][7] * .177441 \
                                + masterDict[5280373][7] * .750
    redWineDict["Tangley Oaks"] = masterDict[5280170][7] * .177441 \
                                  + masterDict[5280233][7] * .750
    redWineDict["Val Beylie"] = masterDict[5280172][7] * .177441 \
                                + masterDict[5280235][7] * .750
    redWineDict["Campolieti"] = masterDict[5280174][7] * .177441 \
                                + masterDict[5280237][7] * .750
    redWineDict["Vina Amalia Reserva Malbec"] = masterDict[5280176][7] * .177441 \
                                                + masterDict[5280239][7] * .750
    redWineDict["Querceto Chianti"] = masterDict[5280178][7] * .177441 \
                                      + masterDict[5280241][7] * .750
    redWineDict["Villa Barbi Rosso"] = masterDict[5280180][7] * .177441 \
                                       + masterDict[5280247][7] * .750
    redWineDict["Belleruche CDR"] = masterDict[5280184][7] * .177441 \
                        + masterDict[5280301][7] * .750
    redWineDict["Lyric Pinot Noir"] = masterDict[5280186][7] * .177441 \
                        + masterDict[5280305][7] * .750
    redWineDict["Tamari Cab"] = masterDict[5280188][7] * .177441 \
                        + masterDict[5280309][7] * .750
    redWineDict["Joel Gott Cab"] = masterDict[5280190][7] * .177441 \
                        + masterDict[5280311][7] * .750
    redWineDict["Oberon Cab"] = masterDict[5280192][7] * .177441 \
                        + masterDict[5280313][7] * .750
    print (whiteWineDict)
    print (redWineDict)

    # # Create file for wine sales and output data.
    # nameWine = startEndDate + '_wineSales.csv'
    # wineSales = open(nameWine,'w')
    # wineSales.write('CONSOLIDATED WINE SALES,,' + startEndDate + '\n\n')
    # wineSales.write('Wine,Bottles Sold,Avg. Price per Bottle\n')
    # for wine in consolidatewine:
    #     bottles = 0
    #     if consolidatewine[wine][0] == 0:
    #         avgp = '0'
    #         pbottles = '0'
    #         outline = wine + ',0,0\n'
    #     else:
    #         bottles = consolidatewine[wine][0] / 750.
    #         pbottles = '{:.2f}'.format(bottles)
    #         avgp = '{:.2f}'.format(consolidatewine[wine][1] / bottles)
    #         outline = wine + ',' + pbottles + ',' + avgp + '\n'
    #     wineSales.write(outline)
    #
    # # Consolidate liquor variants of the same product into
    # # one total quantity, e.g. regular (no suffix), 2oz, Up pour
    # consolidateliquor = {}
    # # Need a separate dictionary for named cocktails
    # for menuitem in alcoholDict:
    #     # make sure there's no trailing white space
    #     item = menuitem.rstrip()
    #     # determine the volume based on regular shot, 2oz or Up
    #     volume = 0
    #     sales = 0
    #     root = ''
    #     if (item[-4:] == ' 2oz'):
    #         root = item[:-4]
    #         volume = alcoholDict[menuitem][0] * 59.1471
    #         sales = alcoholDict[menuitem][1]
    #     elif (item[-3:] == ' Up'):
    #         root = item[:-3]
    #         volume = alcoholDict[menuitem][0] * 88.7206
    #         sales = alcoholDict[menuitem][1]
    #     else:
    #         root = item
    #         if root == 'Martini':
    #             root = 'Vodka'
    #             volume = alcoholDict[menuitem][0] * 88.7206
    #             sales = alcoholDict[menuitem][1]
    #         elif root == 'Bloody Mary':
    #             root = 'Vodka'
    #             volume = alcoholDict[menuitem][0] * 59.1471
    #             sales = alcoholDict[menuitem][1]
    #         elif root[-9:] == 'Margarita':
    #             root = 'Tequila'
    #             volume = alcoholDict[menuitem][0] * 36.96691
    #             sales = alcoholDict[menuitem][1]
    #         elif root == 'Hendricks Mule':
    #             root = 'Hendricks Gin'
    #             volume = alcoholDict[menuitem][0] * 59.1471
    #             sales = alcoholDict[menuitem][1]
    #         elif root == 'Apple Bourbon Cool Toddy':
    #             root = 'Coopers Craft'
    #             volume = alcoholDict[menuitem][0] * 59.1471
    #             sales = alcoholDict[menuitem][1]
    #         elif root == 'Irish Coffee':
    #             root = 'Bushmills'
    #             volume = alcoholDict[menuitem][0] * 36.96691
    #             sales = alcoholDict[menuitem][1]
    #         else:
    #             volume = alcoholDict[menuitem][0] * 36.96691
    #             sales = alcoholDict[menuitem][1]
    #
    #     if (root not in consolidateliquor and root != ''):
    #         consolidateliquor[root] = (volume, sales)
    #     elif (root != ''):
    #         currentVolume = consolidateliquor[root][0]
    #         newVolume = currentVolume + volume
    #         currentSales = consolidateliquor[root][1]
    #         newSales = currentSales + sales
    #         consolidateliquor[root] = (newVolume, newSales)
    #
    # # Create file for liquor, cocktail and beer sales, and output data.
    # lcbFilename = startEndDate + '_LCB.csv'
    # lcbSales = open(lcbFilename,'w')
    # lcbSales.write('LIQUOR COCKTAIL AND BEER SALES,' + startEndDate + '\n\n')
    # lcbSales.write('Liquor,Liters,Shots,Avg price/shot\n')
    # for liquor in consolidateliquor:
    #     liters = 0
    #     if consolidateliquor[liquor][0] == 0:
    #         avgp = '0'
    #         pml = '0'
    #         pshots = '0'
    #         outline = liquor + ',0,0,0\n'
    #     else:
    #         liters = consolidateliquor[liquor][0] / 1000
    #         shots = liters/.03696691
    #         pml = '{:.2f}'.format(liters)
    #         pshots = '{:.2f}'.format(shots)
    #         avgp = '{:.2f}'.format(consolidateliquor[liquor][1] / shots)
    #         outline = liquor + ',' + pml + ',' + pshots + ','+ avgp + '\n'
    #     lcbSales.write(outline)
    #
    # # Write out cocktail sales to the file
    # lcbSales.write('\nCocktail,Sold,Avg price\n')
    # for cocktail in cocktailDict:
    #     if cocktailDict[cocktail][0] == 0:
    #         avgp = '0'
    #         pcount = '0'
    #         outline = cocktail + ',0,0\n'
    #     else:
    #         count = cocktailDict[cocktail][0]
    #         pcount = '{:.2f}'.format(count)
    #         avgp = '{:.2f}'.format(cocktailDict[cocktail][1] / count)
    #         outline = cocktail + ',' + pcount + ',' + avgp + '\n'
    #     lcbSales.write(outline)
    #
    # # Write out beer sales to the file
    # lcbSales.write('\nBeer,Sold,Avg price\n')
    # for bottle in beerDict:
    #     if beerDict[bottle][0] == 0:
    #         avgp = '0'
    #         outline = bottle + ',0,0\n'
    #     else:
    #         count = beerDict[bottle][0]
    #         pcount = '{:.2f}'.format(count)
    #         avgp = '{:.2f}'.format(beerDict[bottle][1] / count)
    #         outline = bottle + ',' + pcount + ','  + avgp + '\n'
    #     lcbSales.write(outline)
    #
    #
    # # Create file for food sales, and output data.
    # foodFilename = startEndDate + '_Food.csv'
    # foodSales = open(foodFilename,'w')
    # foodSales.write('FOOD SALES,' + startEndDate + '\n\n')
    # foodSales.write('Category,Menu Item,Sold,Avg price\n')
    # for plate in foodDict:
    #     parts = plate.split('::')
    #     cat = parts[0]
    #     name = parts[1]
    #     if foodDict[plate][0] == 0:
    #         avgp = 0
    #         outline = plate + ',0,0\n'
    #     else:
    #         count = foodDict[plate][0]
    #         pcount = '{:.2f}'.format(count)
    #         avgp = '{:.2f}'.format(foodDict[plate][1] / count)
    #         outline = cat + ',' + name + ',' + pcount + ',' + avgp + '\n'
    #     foodSales.write(outline)
    #
    return 0

##### _Main_

lineInput = sys.argv
inputFile = lineInput[1]
if len(inputFile) > 0:
    processMenu(inputFile)
    consolidateWine()
else:
    processMenu('data/infile.csv')
