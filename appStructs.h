#ifndef APPSTRUCTS_H
#define APPSTRUCTS_H

#include <QString>
#include <QDateTime>

enum prodCategory {
    bourbon,
    brandy,
    cognac,
    gin,
    liqueur,
    rum,
    scotch,
    tequila,
    vodka,
    whiskey
};

typedef struct productStruct {
    qint32   productID;       // Unique integer key used in database
    QString   labelName;       // Name of product
    QString   prodBarcode;     // String read from manufacturer's barcode
    QString   abcCode;         // Alabama Beverage Control product code
    float     volume;          // in milliliters (ml)
    prodCategory cat;          // category of product
    float     density;         // grams/ml
} *productPtr;

typedef struct inventoryStruct {
    qint32   inventoryID;     // Unique integer key used in database
    qint32   productFK;       // Foreign key reference to product
    QString   invBarcode;      // Barcode created when added to inventory
    bool      inService;
    bool      retired;
    float     fullWeight;      // weight of product and flask in grams
    float     tareWeight;      // weight of empty flask in grams
    QDateTime welcomeDate;     // date product enters into inventory
    QDateTime goodbyeDate;     // date product leaves inventory
} *inventoryPtr;

typedef struct readingStruct {
    qint32    readingID;       // Unique integer key used in database
    qint32    inventoryFK;     // Foreign key reference to inventory
    QDateTime readDate;        // Date of reading
    float     weight;          // reading in grams
} *readingPtr;

#endif // APPSTRUCTS_H
