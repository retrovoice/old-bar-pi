#ifndef APPSTRUCTS_H
#define APPSTRUCTS_H

#include <QString>
#include <QDateTime>

typedef struct productStruct {
    QString   upccode;         // String read from manufacturer's barcode
    QString   label;           // Name of product
    QString   abccode;         // Alabama Beverage Control product code
    qint32    category;        // category of product (references category table)
    float     volume;          // in milliliters (ml)
    float     density;         // grams/ml
} *productPtr;

typedef struct inventoryStruct {
    qint32    id;              // Unique integer key used in database
    qint32    upc;             // Foreign key reference to product upccode
    QString   barcode;         // Barcode created when added to inventory
    bool      retired;         // No longer in inventory
    float     gross;           // weight of product and flask in kilograms
    float     tare;            // weight of empty flask in kilograms
    QDateTime arrival;         // date product enters into inventory
    QDateTime departure;       // date product leaves inventory
} *inventoryPtr;

typedef struct readingsStruct {
    qint32    id;              // Unique integer key used in database
    QString   item;            // Inventory bar code of item weighed
    QDateTime stamp;           // Date of reading
    float     weight;          // reading in grams
} *readingPtr;

#endif // APPSTRUCTS_H
