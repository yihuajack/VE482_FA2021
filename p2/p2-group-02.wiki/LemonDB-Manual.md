# LemonDB Manual
## Table Management Queries
### Load a new table from a file
```
LOAD tableFilePath;
```
Load a table from a file located at `tableFilePath`.
### Copy a table
```
COPYTABLE table newtable;
```
Create a copy of the existing table `table`, and named it `newtable`.
### Dump existing table to file
```
DUMP table filePath;
```
Dump a table `table` into a file located at `filePath`.
### Delete an existing table
```
DROP table;
```
Delete the table `table` along with all its content.
### Clear ane existing table
```
TRUNCATE table;
```
Delete all the content of the table `table`, i.e. removes all the records from this table. The table becomes empty after this operation.
## Data Manipulation Queries
### Delete records from a table
```
DELETE ( ) FROM table;
DELETE ( ) FROM table WHERE ( cond ) ...;
```
Delete rows from the table `table`. If `WHERE` clause is omitted then all rows are considered to match the test. A `WHERE` clause is formed of the `WHERE` keyword followed by multiple *condition tuple*. Each condition tuple is a 3-tuple in the form of `( field op value)`, where `field` is the name of a field, `op` is a comparison operator (one of >, <, =, >=, <=), and `value` is either a tring (`KEY` field) or an integer (other fields).
### Insert new record into a table
```
INSERT ( key value1 value2 ... ) FROM table;
```
Insert the row `( key value1 value2 ... )` into the table `table`. If `KEY` already exists no changes are made to the table.
### Update data in a table
```
UPDATE ( field value ) FROM table WHERE ( cond ) ...;
```
Update the field `field` of the rows satisfying the conditions with new value `value` in `table`.
### Accessing data in a table
```
SELECT ( KEY field ... ) FROM table WHERE ( cond ) ...;
```
For each record satisfying the condition print the fields specified in the `SELECT` clause. Each field that is not a `KEY` may appear at most once.
### Swapping values
```
SWAP ( field1 field2 ) FROM table WHERE ( cond ) ...;
```
This query swaps the values of `field1` and `field2` for the records of `table` that satisfy the given condition.
### Duplicating records
```
DUPLICATE ( ) FROM table WHERE ( cond ) ...;
```
This query copies the records satisfying the condition in table `table`. The affected records are inserted into the table, with key `originalKey_copy`. If a copy of a record already exists the copy is not overwritten, the copy can be duplicated into `originalKey_copy_copy`.
### Summing records
```
SUM ( fields ... ) FROM table WHERE ( cond ) ...;
```
This query aggregates records that satisfies the given conditions. The `SUM` clause sums the values of one or more fields given in `fields` over all the affected records. The `KEY` field cannot be summed over.
### Counting records
```
COUNT ( ) FROM table WHERE ( cond ) ...;
```
This query counts the number of records that satisfies the conditions.
### Finding minima / maxima
```
MIN ( fields ... ) FROM table WHERE ( cond ) ...;
MAX ( fields ... ) FROM table WHERE ( cond ) ...;
```
The `MIN` clause finds the minimum value among all affected records for the values of one or more fields given in `fields`. The `MAX` clause finds the maximum value among all affected records for the values of one or more fields given in `fields`
### Basic Arithmetics
```
ADD ( fields ... destField ) FROM table WHERE ( cond ) ...;
SUB ( fieldSrc fields ... destField ) FROM table WHERE ( cond ) ...;
```
The `ADD` clause sums up one or more fields given in `fields` and store the result in the `destField`. The `SUB` clause subtracts zero or more values of `fields` field, from the `fieldSrc` field, and stores the result in `destField`.
## Utilities
### Read queries from a new file
```
LISTEN ( path_of_file );
```
Use a thread to read queries from the file pointed to by `path_of_file`. Either an abosulte or a relative path can be provided.
### Quit database
```
QUIT;
```
Quit the database. Wait for running queries to complete.