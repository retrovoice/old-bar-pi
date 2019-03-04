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

    fileTuple3      = csvFile.rpartition('/')
    fileName        = fileTuple3[-1]
    dateString_wext = fileName.lstrip('pmix_')
    dateString      = dateString_wext.rstrip('.csv')
    print('Processing data for period: ' + dateString)

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
                # Strip off the leading 4 and all 0s that follow.  This gets rid of
                # the '4000000..' that all the IDs have, and leaves the numbers that
                # form a unique ID for the item.
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
    return dateString


def consolidateWine(dateString):

    # This routine consolidates the wine by the bottle and by the
    # glass sales into a single quantity.  It uses a dictionary
    # with the name of the wine as the key.

    # Factor for glasses per bottle
    toBottle = 177.441/750.

    whiteWineDict["Cuvee Van Gogh"] = masterDict[5280259][7] * toBottle \
                                      + masterDict[5280369][7]
    whiteWineDict["Dr L Reisling"] = masterDict[209729656][7] * toBottle \
        + masterDict[161452344][7]
    whiteWineDict["Foxglove Chard"] = masterDict[198315560][7] * toBottle \
        + masterDict[198324707][7]
    whiteWineDict["Heron Chardonnay"] = masterDict[5280271][7] * toBottle \
                                        + masterDict[5280387][7]
    whiteWineDict["Reverdy Sancerre"] = masterDict[5280385][7]
    whiteWineDict["Laguna Chard"] = masterDict[209729658][7] * toBottle \
        + masterDict[161452340][7]
    whiteWineDict["Luminus Chardonnay"] = masterDict[5280273][7] * toBottle \
                                          + masterDict[5280391][7]
    whiteWineDict["Lyric Chardonnay"] = masterDict[20379822][7] * toBottle \
                                        + masterDict[5280389][7]
    whiteWineDict["Montevina White Zin"] = masterDict[20379824][7] * toBottle \
                                           + masterDict[20379189][7]
    whiteWineDict["North by Northwest"] = masterDict[209729654][7] * toBottle \
        + masterDict[161452342][7]
    whiteWineDict["Promesso"] = masterDict[5280263][7] * toBottle \
                                + masterDict[5280373][7]
    whiteWineDict["Sambardier Chard"] = masterDict[498698770][7] * toBottle \
                                 + masterDict[498706877][7]
    whiteWineDict["Soave DOP"] = masterDict[5280267][7] * toBottle \
                                 + masterDict[5280379][7]
    whiteWineDict["Terlato PG"] = masterDict[5280375][7]
    whiteWineDict["Whitehaven"] = masterDict[5280269][7] * toBottle \
                                  + masterDict[5280383][7]


    whiteWineDict["Bollicini Prosecco"] = masterDict[517823884][7] * toBottle \
                                          + masterDict[498591695][7] *.15 \
                                          + masterDict[5279953][7] *.15 \
                                          + masterDict[517818526][7]
    whiteWineDict["Luccio Moscato"] = masterDict[5280591][7] * toBottle \
        + masterDict[5280642][7]
    whiteWineDict["Nino Franco Rustico"] = masterDict[5280589][7] * toBottle \
        + masterDict[5280646][7]
    whiteWineDict["Veuve Clicquot"] = masterDict[5280648][7]
    whiteWineDict["Villa Sandi"] = masterDict[5280638][7]
    whiteWineDict["Wycliff"] = masterDict[5280593][7] * toBottle \
        + masterDict[5280640][7]


    redWineDict["Campolieti"] = masterDict[5280174][7] * toBottle \
                                + masterDict[5280237][7]
    redWineDict["Canti Barolo"] = masterDict[311593394][7]
    redWineDict["Chateau Briot"] = masterDict[5280303][7]
    redWineDict["Chateau Vignot"] = masterDict[5280323][7]
    redWineDict["Durigutti Cab Sauv"] = masterDict[161440162][7] * toBottle \
                                        + masterDict[209716179][7]
    redWineDict["Emblem Cab"] = masterDict[5280331][7]
    redWineDict["Emerish Pinot Noir"] = masterDict[5280307][7]
    redWineDict["Federalist Zinfandel"] = masterDict[20378197][7] * toBottle \
                                          + masterDict[20376740][7]
    redWineDict["Hahn SLH Pinot Noir"] = masterDict[198321005][7] * toBottle \
                                         + masterDict[198328854][7]
    redWineDict["Hartford Zin"] = masterDict[5280317][7]
    redWineDict["Joel Gott Cab"] = masterDict[5280190][7] * toBottle \
                                   + masterDict[5280311][7]
    redWineDict["Knights Valley Cab"] = masterDict[5280315][7]
    redWineDict["La Loyane CDR"] = masterDict[311425010][7] * toBottle \
                                   + masterDict[311412437][7]
    redWineDict["Levendi Pinot Noir"] = masterDict[5280329][7]
    redWineDict["Luigi Righetti Amarone"] = masterDict[20376742][7]
    redWineDict["Lyric Pinot Noir"] = masterDict[5280186][7] * toBottle \
                                      + masterDict[5280305][7]
    redWineDict["Modus"] = masterDict[5280245][7]
    redWineDict["Oberon Cab"] = masterDict[5280192][7] * toBottle \
                                + masterDict[5280313][7]
    redWineDict["Quantum"] = masterDict[5280327][7]
    redWineDict["Querceto Chianti"] = masterDict[5280178][7] * toBottle \
                                      + masterDict[5280241][7]
    redWineDict["Sangue Di Guida"] = masterDict[20378199][7] * toBottle \
                                     + masterDict[20376746][7]
    redWineDict["Stags Leap Petite Syrah"] = masterDict[169324410][7]
    redWineDict["Tamari Cab"] = masterDict[5280188][7] * toBottle \
                                + masterDict[5280309][7]
    redWineDict["Tangley Oaks"] = masterDict[5280170][7] * toBottle \
                                  + masterDict[5280233][7]
    redWineDict["Tenuta di Arceno"] = masterDict[5280243][7]
    redWineDict["Val Beylie"] = masterDict[5280172][7] * toBottle \
                                + masterDict[5280235][7]
    redWineDict["Velvet Devil"] = masterDict[37305962][7] * toBottle \
                                  + masterDict[33865502][7]
    redWineDict["Vina Amalia Reserva Malbec"] = masterDict[5280176][7] * toBottle \
                                                + masterDict[5280239][7]
    redWineDict["Levendi Pinot Noir"] = masterDict[5280329][7]

    # Create file for wine sales and output data.
    nameWine = dateString + '_WineSales.csv'
    wineSales = open(nameWine,'w')
    wineSales.write('CONSOLIDATED WINE SALES\n')
    wineSales.write(dateString + '\n\n')
    wineSales.write('Wine,Bottles Sold\n')
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
    for wine in redWineDict:
        bottles = 0
        #avgp = '0'
        pbottles = '0'
        outline = ''
        if redWineDict[wine] == 0.:
            outline = wine + ',0\n'
        else:
            bottles = redWineDict[wine]
            pbottles = '{:.2f}'.format(bottles)
            #avgp = '{:.2f}'.format(redWineDict[wine][1] / bottles)
            #outline = wine + ',' + pbottles + ',' + avgp + '\n'
            outline = wine + ',' + pbottles + '\n'
        wineSales.write(outline)
    wineSales.close()
    return 0

##### _Main_

lineInput = sys.argv
inputFile = lineInput[1]
if len(inputFile) > 0:
    consolidateWine(processMenu(inputFile))
else:
    processMenu('data/infile.csv')
