__author__ = 'mcconnelldj'

import csv
import locale
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
                longID = line[0].lstrip('4')
                shortID = longID.lstrip('0')
                if shortID:
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

    # Factor for glasses per bottle
    toBottle = 177.441/750.
    #print(masterDict["400000000005280263"][8] + ", " + masterDict["400000000005280373"][8])

    whiteWineDict["Promesso"] = masterDict[5280263][7] * toBottle \
                                + masterDict[5280373][7]
    whiteWineDict["Montevina White Zin"] = masterDict[20379824][7] * toBottle \
                                           + masterDict[20379189][7]
    whiteWineDict["Cuvee Van Gogh"] = masterDict[5280259][7] * toBottle \
                                      + masterDict[5280369][7]
    whiteWineDict["Selbach Incline"] = masterDict[5280261][7] * toBottle \
                                       + masterDict[5280371][7]
    whiteWineDict["Torrontes La Yunta"] = masterDict[5280265][7] * toBottle \
                                          + masterDict[5280377][7]
    whiteWineDict["Soave DOP"] = masterDict[5280267][7] * toBottle \
                                 + masterDict[5280379][7]
    whiteWineDict["Whitehaven"] = masterDict[5280269][7] * toBottle \
                                  + masterDict[5280383][7]
    whiteWineDict["Heron Chardonnay"] = masterDict[5280271][7] * toBottle \
                                        + masterDict[5280387][7]
    whiteWineDict["Lyric Chardonnay"] = masterDict[20379822][7] * toBottle \
                                        + masterDict[5280389][7]
    whiteWineDict["Luminus Chardonnay"] = masterDict[5280273][7] * toBottle \
                                          + masterDict[5280391][7]
    whiteWineDict["Conumdrum"] = masterDict[113382578][7] * toBottle \
                                  + masterDict[113386926][7]

    redWineDict["Tangley Oaks"] = masterDict[5280170][7] * toBottle \
                                  + masterDict[5280233][7]
    redWineDict["Val Beylie"] = masterDict[5280172][7] * toBottle \
                                + masterDict[5280235][7]
    redWineDict["Campolieti"] = masterDict[5280174][7] * toBottle \
                                + masterDict[5280237][7]
    redWineDict["Vina Amalia Reserva Malbec"] = masterDict[5280176][7] * toBottle \
                                                + masterDict[5280239][7]
    redWineDict["Querceto Chianti"] = masterDict[5280178][7] * toBottle \
                                      + masterDict[5280241][7]
    redWineDict["Villa Barbi Rosso"] = masterDict[5280180][7] * toBottle \
                                       + masterDict[5280247][7]
    redWineDict["Belleruche CDR"] = masterDict[5280184][7] * toBottle \
                                    + masterDict[5280301][7]
    redWineDict["Lyric Pinot Noir"] = masterDict[5280186][7] * toBottle \
                                      + masterDict[5280305][7]
    redWineDict["Tamari Cab"] = masterDict[5280188][7] * toBottle \
                                + masterDict[5280309][7]
    redWineDict["Joel Gott Cab"] = masterDict[5280190][7] * toBottle \
                                   + masterDict[5280311][7]
    redWineDict["Oberon Cab"] = masterDict[5280192][7] * toBottle \
                                + masterDict[5280313][7]
    redWineDict["Federalist Zinfandel"] = masterDict[20378197][7] * toBottle \
                                          + masterDict[20376740][7]
    redWineDict["Sangue Di Guida"] = masterDict[20378199][7] * toBottle \
                                     + masterDict[20376746][7]
    redWineDict["Beaujolais"] = masterDict[20378301][7] * toBottle \
                                + masterDict[20376736][7]
    redWineDict["Durigutti Cabernet Franc"] = masterDict[21944774][7] * toBottle \
                                              + masterDict[21941608][7]
    redWineDict["Dos Fincas Cab Sauv Malbec"] = masterDict[21944776][7] * toBottle \
                                                + masterDict[21941610][7]
    redWineDict["Velvet Devil"] = masterDict[37305962][7] * toBottle \
                                  + masterDict[33865502][7]
    print (whiteWineDict)
    print (redWineDict)

    # Create file for wine sales and output data.
    nameWine = 'WineSales.csv'
    wineSales = open(nameWine,'w')
    wineSales.write('CONSOLIDATED WINE SALES\n\n')
    wineSales.write('Wine,Bottles Sold,Avg. Price per Bottle\n')
    for wine in whiteWineDict:
        bottles = 0
        #avgp = '0'
        pbottles = '0'
        outline = ''
        if whiteWineDict[wine] == 0.:
            outline = wine + ',0\n'
        else:
            bottles = whiteWineDict[wine]
            pbottles = '{:.2f}'.format(bottles)
            #avgp = '{:.2f}'.format(whiteWineDict[wine][1] / bottles)
            #outline = wine + ',' + pbottles + ',' + avgp + '\n'
            outline = wine + ',' + pbottles + '\n'
        wineSales.write(outline)
    wineSales.close()
    return 0

##### _Main_

lineInput = sys.argv
inputFile = lineInput[1]
if len(inputFile) > 0:
    processMenu(inputFile)
    consolidateWine()
else:
    processMenu('data/infile.csv')
