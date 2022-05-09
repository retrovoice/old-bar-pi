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
cardDict = {}

locale.setlocale(locale.LC_ALL, '')

def processMenu(csvFile):
    with open(csvFile) as f:

        reader = csv.reader(f)
        # read and discard first line
        next(reader)

        # Iterate over lines in the CSV file
        for line in reader:

            # The elements in the list 'line' are:
            # line[0] Location
            # line[1] Card_ID
            # line[2] Account_ID
            # line[3] ClassicCard? boolean
            # line[4] Deactivated? boolean
            # line[5] Imported_Number
            # line[6] Card_Number
            # line[7] First_Trans_Date
            # line[8] Initial_Amount
            # line[9] Last_Trans_Date
            # line[10] Current_Balance

            Imported_Number = line[5]
            Card_Number = line[6]
            Current_Balance = line[10]

            if Imported_Number:
                cardDict[Imported_Number] = Current_Balance
            else:
                cardDict[Card_Number] = Current_Balance

    with open('cardlist.csv', 'w', newline='') as fout:
        writer = csv.writer(fout)
        for Cnumber in cardDict:
            #print(f'{Cnumber},{cardDict[Cnumber]}')
            print (Cnumber,',',cardDict[Cnumber])
            writer.writerow ([Cnumber,cardDict[Cnumber]])

    return 0

##### _Main_

processMenu('data/GiftCards.csv')
