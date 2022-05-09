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
            # line[1] Item ID            in dictionary
            # line[2] Parent ID
            # line[3] Menu Name
            # line[4] Menu Group
            # line[5] Subgroup
            # line[6] Menu Item         ->[0]
            # line[7] Avg Price
            # line[8] Item Qty w/voids
            # line[9] Percent Ttl Qty
            # line[10] Gross Amount
            # line[11] Percent Ttl Amt
            # line[12] Item Qty         ->[1]
            # line[13] Gross Amount     ->[2]
            # line[14] Void Qty         ->[3]
            # line[15] Void Amt         ->[4]
            # line[16] Discount Amount  ->[5]
            # line[17] Net Amount       ->[6]
            # line[18] Order Count

            if line[0]:
                # Convert Master ID text value to integer for use as dictionary key
                # Strip off the leading 6 and all 0s that follow.  This gets rid of
                # the '6000000..' that all the IDs have, and leaves the numbers that
                # form a unique ID for the item.
                longID = line[1].lstrip('6')
                shortID = longID.lstrip('0')
                if shortID:
                    int_key = locale.atoi(shortID)
                    #print(int_key)

                # Check the Menu Group (line[4]) to see if it's wine
                if line[4] == 'RED WINE' or line[4] == 'WHITE WINE' or line[4] == 'SPARKLING/ROSE':

                    # This is a new key, so simply add it to the dictionary
                    if int_key not in masterDict and int_key > 0:
                        masterDict[int_key] = [line[6],
                                            locale.atof(line[12]),locale.atof(line[13]),
                                            locale.atof(line[14]),locale.atof(line[15]),
                                            locale.atof(line[16]),locale.atof(line[17])]


                    # There are some keys for open item buttons we don't care about
                    elif int_key > 0:
                        # This key is already in dictionary, so update the entry by adding
                        # this item's Qty/Amount to previous total.

                        print ("Duplicate key:  " + int_key + ", for menu item " + masterDict[int_key][0])
                        # Note indexes in dictionary different than CVS file line
                        updatedLine = [masterDict[int_key][0],
                                    masterDict[int_key][1] + locale.atof(line[12]),
                                    masterDict[int_key][2] + locale.atof(line[13]),
                                    masterDict[int_key][3] + locale.atof(line[14]),
                                    masterDict[int_key][4] + locale.atof(line[15]),
                                    masterDict[int_key][5] + locale.atof(line[16]),
                                    masterDict[int_key][6] + locale.atof(line[17])]
                        masterDict[int_key] = updatedLine
    #print(masterDict)
    return dateString


def consolidateWine(dateString):

    # This routine consolidates the wine by the bottle and by the
    # glass sales into a single quantity.  It uses a dictionary
    # with the name of the wine as the key.

    # Factor for glass as portion of a bottle
    toBottle = 0.25

    redWineDict["Durigutti Bonardo"] = ( masterDict[126717776][1] * toBottle \
                                  + masterDict[126675701][1], \
                                  masterDict[126717776][6] \
                                  + masterDict[126675701][6] )
    redWineDict["Durigutti Cab Franc"] = ( masterDict[108923090][1] * toBottle \
                                  + masterDict[126662461][1], \
                                  masterDict[108923090][6] \
                                  + masterDict[126662461][6] )
    redWineDict["Elqui Carmenere"] = ( masterDict[126700375][1] * toBottle \
                                  + masterDict[86838560][1], \
                                  masterDict[134741583][6] \
                                  + masterDict[86838560][6] )
    redWineDict["Fernandez Gomez Tempranillo"] = ( masterDict[226472787][1] * toBottle \
                                  + masterDict[226396480][1], \
                                  masterDict[226472787][6] \
                                  + masterDict[226396480][6] )
    redWineDict["Haut Livigne"] = ( masterDict[126707734][1] * toBottle \
                                  + masterDict[126666591][1], \
                                  masterDict[126707734][6] \
                                  + masterDict[126666591][6] )
    redWineDict["Hendry HRW Zin"] = ( masterDict[138978515][1] * toBottle \
                                  + masterDict[139085317][1], \
                                  masterDict[138978515][6] \
                                  + masterDict[139085317][6] )
    redWineDict["Il Nero di Casanova"] = ( masterDict[134751298][1] * toBottle \
                                  + masterDict[134727097][1], \
                                  masterDict[134751298][6] \
                                  + masterDict[134727097][6] )
    redWineDict["La Loyane Lirac"] = ( masterDict[1276497518][1] * toBottle \
                                  + masterDict[1276474213][1], \
                                  masterDict[1276497518][6] \
                                  + masterDict[1276474213][6] )
    redWineDict["Querceto Chianti"] = ( masterDict[126701381][1] * toBottle \
                                  + masterDict[126657095][1], \
                                  masterDict[126701381][6] \
                                  + masterDict[126657095][6] )
    redWineDict["Seghesio Red Zin"] = ( masterDict[126713038][1] * toBottle \
                                  + masterDict[126586184][1], \
                                  masterDict[126713038][6] \
                                  + masterDict[126586184][6] )
    redWineDict["Tangley Oaks Merlot"] = ( masterDict[126697904][1] * toBottle \
                                  + masterDict[126655512][1], \
                                  masterDict[126697904][6] \
                                  + masterDict[126655512][6] )
    redWineDict["Vinalba"] = ( masterDict[126710903][1] * toBottle \
                                  + masterDict[126668956][1], \
                                  masterDict[126710903][6] \
                                  + masterDict[126668956][6] )
    redWineDict["La Loyane CDR"] = ( masterDict[226476258][1] * toBottle \
                                  + masterDict[226401294][1], \
                                  masterDict[226476258][6] \
                                  + masterDict[226401294][6] )
    redWineDict["Bacchus Cab"] = ( masterDict[134741583][1] * toBottle \
                                  + masterDict[134684785][1], \
                                  masterDict[134741583][6] \
                                  + masterDict[134684785][6] )
    redWineDict["Daou Cab"] = ( masterDict[126695589][1] * toBottle \
                                  + masterDict[126582598][1], \
                                  masterDict[126695589][6] \
                                  + masterDict[126582598][6] )
    redWineDict["Joel Gott"] = ( masterDict[126702220][1] * toBottle \
                                  + masterDict[126588031][1], \
                                  masterDict[126702220][6] \
                                  + masterDict[126588031][6] )
    redWineDict["Oberon Cab"] = ( masterDict[108923080][1] * toBottle \
                                  + masterDict[126600823][1], \
                                  masterDict[108923080][6] \
                                  + masterDict[126600823][6] )
    redWineDict["Lyric Pinot Noir"] = ( masterDict[1394304966][1] * toBottle \
                                  + masterDict[1394368994][1], \
                                  masterDict[1394304966][6] \
                                  + masterDict[1394368994][6] )
    redWineDict["Meiomi PN"] = ( masterDict[126698677][1] * toBottle \
                                  + masterDict[126616181][1], \
                                  masterDict[126698677][6] \
                                  + masterDict[126616181][6] )
    redWineDict["Fableist PN"] = ( masterDict[108923074][1] * toBottle \
                                  + masterDict[126617293][1], \
                                  masterDict[108923074][6] \
                                  + masterDict[126617293][6] )
    redWineDict["Hey Malbec"] = ( masterDict[108923076][1] * toBottle \
                                  + masterDict[126631693][1], \
                                  masterDict[108923076][6] \
                                  + masterDict[126631693][6] )
    redWineDict["Vina Amalia Malbec"] = ( masterDict[126709461][1] * toBottle \
                                  + masterDict[126624713][1], \
                                  masterDict[126709461][6] \
                                  + masterDict[126624713][6] )
    redWineDict["Delechaneau Ad Libitum"] = ( masterDict[126674376][1], \
                                  masterDict[126674376][6])
    redWineDict["Albe Barolo"] = ( masterDict[126654733][1], \
                                  masterDict[126654733][6])
    redWineDict["Chateauneuf du Pape"] = ( masterDict[126577844][1], \
                                  masterDict[126577844][6])
    redWineDict["Fernandez Gomez Garnacha"] = ( masterDict[126665706][1], \
                                  masterDict[126665706][6])
    redWineDict["Frescobaldi Brunello"] = ( masterDict[126652489][1], \
                                  masterDict[126652489][6])
    redWineDict["Prunotto Barolo"] = ( masterDict[126653764][1], \
                                  masterDict[126653764][6])
    redWineDict["Textbook Merlot"] = ( masterDict[134692033][1], \
                                  masterDict[134692033][6])
    redWineDict["Deux Anes"] = ( masterDict[126658957][1], \
                                  masterDict[126658957][6])
    redWineDict["Eberle Cab"] = ( masterDict[134690352][1], \
                                  masterDict[134690352][6])
    redWineDict["Volker Estate Cab"] = ( masterDict[86790527][1], \
                                  masterDict[86790527][6])
    redWineDict["Daou Cab Riserva"] = ( masterDict[1918727844][1], \
                                  masterDict[1918727844][6])
    redWineDict["Ermisch PN"] = ( masterDict[126619457][1], \
                                  masterDict[126619457][6])
    redWineDict["Durigutti Riserva Malbec"] = ( masterDict[126638299][1], \
                                  masterDict[126638299][6])
    redWineDict["Carmelo Patti Malbec"] = ( masterDict[126630355][1], \
                                  masterDict[126630355][6])


    whiteWineDict["Bacchus Chard"] = ( masterDict[134743983][1] * toBottle \
                                        + masterDict[134708305][1], \
                                        masterDict[134743983][6] \
                                        + masterDict[134708305][6] )

    # Create file for wine sales and output data.
    nameWine = dateString + '_WineSales.csv'
    wineSales = open(nameWine,'w')
    wineSales.write('CONSOLIDATED WINE SALES\n')
    wineSales.write(dateString + '\n\n')
    wineSales.write('Wine,Bottles Sold\n')
    for wine in whiteWineDict:
        bottles = 0
        avgp = '0'
        pbottles = '0'
        outline = ''
        if whiteWineDict[wine][0] == 0.:
            outline = wine + ',0,0\n'
        else:
            bottles = whiteWineDict[wine][0]
            pbottles = '{:.2f}'.format(bottles)
            avgp = '{:.2f}'.format(whiteWineDict[wine][1] / bottles)
            outline = wine + ',' + pbottles + ',' + avgp + '\n'
            #outline = wine + ',' + pbottles + '\n'
        wineSales.write(outline)
    for wine in redWineDict:
        bottles = 0
        avgp = '0'
        pbottles = '0'
        outline = ''
        if redWineDict[wine][0] == 0.:
            outline = wine + ',0,0\n'
        else:
            bottles = redWineDict[wine][0]
            pbottles = '{:.2f}'.format(bottles)
            avgp = '{:.2f}'.format(redWineDict[wine][1] / bottles)
            outline = wine + ',' + pbottles + ',' + avgp + '\n'
            #outline = wine + ',' + pbottles + '\n'
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
