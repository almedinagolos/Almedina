------------------------------------------------
--1
/*
a) Kreirati bazu podataka pod vlastitim brojem indeksa.
*/
CREATE DATABASE JUL20
USE JUL20
--Prilikom kreiranja tabela voditi računa o međusobnom odnosu između tabela.
/*
b) Kreirati tabelu radnik koja će imati sljedeću strukturu:
	- radnikID, cjelobrojna varijabla, primarni ključ
	- drzavaID, 15 unicode karaktera
	- loginID, 256 unicode karaktera
	- god_rod, cjelobrojna varijabla
	- spol, 1 unicode karakter
*/
CREATE TABLE Radnik(
RadnikID INT CONSTRAINT PK_RadnikID PRIMARY KEY ,
DrzavaID NVARCHAR(15),
LoginID NVARCHAR(265),
God_Rod INT,
Spol NVARCHAR(1)
)
/*
c) Kreirati tabelu nabavka koja će imati sljedeću strukturu:
	- nabavkaID, cjelobrojna varijabla, primarni ključ
	- status, cjelobrojna varijabla
	- radnikID, cjelobrojna varijabla
	- br_racuna, 15 unicode karaktera
	- naziv_dobavljaca, 50 unicode karaktera
	- kred_rejting, cjelobrojna varijabla
*/
CREATE TABLE Nabavka(
NabavkaID INT CONSTRAINT PK_NabavkaID PRIMARY KEY ,
Status INT,
RadnikID INT,
Br_Racuna NVARCHAR(15),
Naziv_Dobavljaca NVARCHAR(50),
Kred_Rejting INT,
CONSTRAINT FK_RadnikID FOREIGN KEY (RadnikID) REFERENCES Radnik(RadnikID)
)
/*
c) Kreirati tabelu prodaja koja će imati sljedeću strukturu:
	- prodajaID, cjelobrojna varijabla, primarni ključ, inkrementalno punjenje sa početnom vrijednošću 1, samo neparni brojevi
	- prodavacID, cjelobrojna varijabla
	- dtm_isporuke, datumsko-vremenska varijabla
	- vrij_poreza, novčana varijabla
	- ukup_vrij, novčana varijabla
	- online_narudzba, bit varijabla sa ograničenjem kojim se mogu unijeti samo cifre 0 ili 1
*/
--10 bodova
CREATE TABLE Prodaja(
ProdajaID INT CONSTRAINT PK_ProdajaID PRIMARY KEY IDENTITY(1,2),
ProdavacID INT,
Dtm_Isporuke DATETIME,
Vrije_Poreza MONEY,
Ukup_Vrij MONEY,
Online_Narudzba BIT CONSTRAINT CK_OnlineNarudzba CHECK (Online_Narudzba = 0 OR Online_Narudzba = 1),
CONSTRAINT FK_ProdavacID FOREIGN KEY (ProdavacID) REFERENCES Radnik(RadnikID)
)


--------------------------------------------
--2. Import podataka
/*
a) Iz tabele Employee iz šeme HumanResources baze AdventureWorks2017 u tabelu radnik importovati podatke po sljedećem pravilu:
	- BusinessEntityID -> radnikID
	- NationalIDNumber -> drzavaID
	- LoginID -> loginID
	- godina iz kolone BirthDate -> god_rod
	- Gender -> spol
*/
-------------------------------------------------
INSERT INTO Radnik
SELECT E.BusinessEntityID, E.NationalIDNumber, E.LoginID, YEAR(E.BirthDate), E.Gender
FROM AdventureWorks2017.HumanResources.Employee AS E 

/*
b) Iz tabela PurchaseOrderHeader i Vendor šeme Purchasing baze AdventureWorks2017 u tabelu nabavka importovati podatke po sljedećem pravilu:
	- PurchaseOrderID -> dobavljanjeID
	- Status -> status
	- EmployeeID -> radnikID
	- AccountNumber -> br_racuna
	- Name -> naziv_dobavljaca
	- CreditRating -> kred_rejting
*/
INSERT INTO Nabavka
SELECT POH.PurchaseOrderID, POH.Status, POH.EmployeeID, V.AccountNumber, V.Name, V.CreditRating
FROM AdventureWorks2017.Purchasing.PurchaseOrderHeader AS POH INNER JOIN AdventureWorks2017.Purchasing.Vendor AS V ON
V.BusinessEntityID = POH.VendorID
/*
c) Iz tabele SalesOrderHeader šeme Sales baze AdventureWorks2017
u tabelu prodaja importovati podatke po sljedećem pravilu:
	- SalesPersonID -> prodavacID
	- ShipDate -> dtm_isporuke
	- TaxAmt -> vrij_poreza
	- TotalDue -> ukup_vrij
	- OnlineOrderFlag -> online_narudzba
*/
--10 bodova
INSERT INTO Prodaja 
SELECT H.SalesPersonID, H.ShipDate, H.TaxAmt, H.TotalDue, H.OnlineOrderFlag
FROM AdventureWorks2017.Sales.SalesOrderHeader AS H

SELECT * FROM Prodaja
------------------------------------------
--3.
/*
a) U tabeli radnik dodati kolonu st_kat (starosna kategorija), tipa 3 karaktera.
*/

/*
b) Prethodno kreiranu kolonu popuniti po principu:
	starosna kategorija		uslov
	I						osobe do 30 godina starosti (uključuje se i 30)
	II						osobe od 31 do 49 godina starosti
	III						osobe preko 50 godina starosti
*/

/*
c) Neka osoba sa navršenih 65 godina starosti odlazi u penziju.
Prebrojati koliko radnika ima 10 ili manje godina do penzije.
Rezultat upita isključivo treba biti poruka 
'Broj radnika koji imaju 10 ili manje godina do penzije je ' nakon čega slijedi prebrojani broj.
Neće se priznati rješenje koje kao rezultat upita vraća više kolona.
*/
--15 bodova
--A
ALTER TABLE Radnik
ADD St_Kat NVARCHAR(3)
--B
UPDATE Radnik
SET St_Kat = 'I'
WHERE YEAR(GETDATE()) - God_Rod <= 30
UPDATE Radnik
SET St_Kat = 'II'
WHERE YEAR(GETDATE()) - God_Rod > 30 AND YEAR(GETDATE()) - God_Rod <= 49 
UPDATE Radnik
SET St_Kat = 'III'
WHERE YEAR(GETDATE()) - God_Rod >= 50

SELECT * FROM Radnik  WHERE YEAR(GETDATE()) - God_Rod >= 55
--C
SELECT 'Broj radnika koji imaju 10 ili manje godina do penzije je ' + CONVERT(NVARCHAR,COUNT(RadnikID)) AS Informacija
FROM Radnik
WHERE YEAR(GETDATE()) - God_Rod >= 55
------------------------------------------
--4.
/*
a) U tabeli prodaja kreirati kolonu stopa_poreza (10 unicode karaktera)
*/
ALTER TABLE Prodaja
ADD Stopa_Poreza NVARCHAR(10)

/*
b) Prethodno kreiranu kolonu popuniti kao količnik vrij_poreza i ukup_vrij,
Stopu poreza izraziti kao cijeli broj sa oznakom %, pri čemu je potrebno 
da između brojčane vrijednosti i znaka % bude prazno mjesto. (npr. 14.00 %)
*/
--10 bodova
UPDATE Prodaja
SET Stopa_Poreza = CONVERT(NVARCHAR,(Vrije_Poreza / Ukup_Vrij)*100) + ' %'

SELECT * FROM Prodaja
-----------------------------------------
--5.
/*
a)
Koristeći tabelu nabavka kreirati pogled view_slova sljedeće strukture:
	- slova
	- prebrojano, prebrojani broj pojavljivanja slovnih dijelova podatka u koloni br_racuna. 
b)
Koristeći pogled view_slova odrediti razliku vrijednosti između prebrojanih i srednje vrijednosti kolone.
Rezultat treba da sadrži kolone slova, prebrojano i razliku.
Sortirati u rastućem redolsijedu prema razlici.
*/
SELECT * FROM Nabavka
--10 bodova
CREATE VIEW view_slova
AS
SELECT SUBSTRING(Br_Racuna, 0, LEN(Br_Racuna) - 3) AS Slova, COUNT(*) AS Prebrojano
FROM Nabavka
GROUP BY SUBSTRING(Br_Racuna, 0, LEN(Br_Racuna) - 3)

--B
SELECT Slova, Prebrojano, Prebrojano - (SELECT AVG(Prebrojano) FROM view_slova) AS Razlika
FROM view_slova
ORDER BY Razlika
-----------------------------------------
--6.
/*
a) Koristeći tabelu prodaja kreirati pogled view_stopa sljedeće strukture:
	- prodajaID
	- stopa_poreza
	- stopa_num, u kojoj će bit numerička vrijednost stope poreza 
b)
Koristeći pogled view_stopa, a na osnovu razlike između vrijednosti u koloni stopa_num i 
srednje vrijednosti stopa poreza za svaki proizvodID navesti poruku 'manji', odnosno, 'veći'. 
*/
--12 bodova
--A
CREATE VIEW view_stopa
AS
SELECT ProdajaID, Stopa_Poreza, CONVERT(FLOAT,SUBSTRING(Stopa_Poreza, 0 , LEN(Stopa_Poreza)- 1)) AS Stopa_Num
FROM Prodaja
--B
SELECT *, 'MANJI'
FROM view_stopa
WHERE Stopa_Num < (SELECT AVG(Stopa_Num) FROM view_stopa) 
SELECT *, 'VECI'
FROM view_stopa
WHERE Stopa_Num > (SELECT AVG(Stopa_Num) FROM view_stopa) 
 --ILI
SELECT *,
CASE
WHEN Stopa_Num < (SELECT AVG(Stopa_Num) FROM view_stopa) THEN 'MANJI'
WHEN Stopa_Num > (SELECT AVG(Stopa_Num) FROM view_stopa)  THEN 'VECI' END
FROM view_stopa
------------------------------------------
--7.
/*
Koristeći pogled view_stopa_poreza kreirati proceduru proc_stopa_poreza
tako da je prilikom izvršavanja moguće unijeti bilo koji broj parametara 
(možemo ostaviti bilo koji parametar bez unijete vrijednosti),
pri čemu će se prebrojati broja zapisa po stopi poreza uz uslova 
da se dohvate samo oni zapisi u kojima je stopa poreza veća od 10 %.
Proceduru pokrenuti za sljedeće vrijednosti:
	- stopa poreza = 12, 15 i 21 
*/
--10 bodova
ALTER PROCEDURE proc_stopa_poreza
(
@ProdajaID INT = NULL,
@Stopa_Poreza NVARCHAR(10) = NULL,
@Stopa_Num FLOAT = NULL
)
AS
BEGIN
SELECT COUNT(*) AS PR
FROM view_stopa
WHERE (ProdajaID = @ProdajaID OR Stopa_Poreza = @Stopa_Poreza OR Stopa_Num = @Stopa_Num) AND Stopa_Num > 10
END

EXEC proc_stopa_poreza @Stopa_Poreza = '12.11 %'

SELECT ProdajaID, Stopa_Poreza, Stopa_Num, COUNT(Stopa_Poreza) AS PR
FROM view_stopa
WHERE  Stopa_Num > 10
GROUP BY ProdajaID, Stopa_Poreza, Stopa_Num

---------------------------------------------------------------------------------------------------
--8.
/*
Kreirati proceduru proc_prodaja kojom će se izvršiti 
promjena vrijednosti u koloni online_narudzba tabele prodaja. 
Promjena će se vršiti tako što će se 0 zamijeniti sa NO, a 1 sa YES. 
Pokrenuti proceduru kako bi se izvršile promjene, a nakon toga onemogućiti 
da se u koloni unosi bilo kakva druga vrijednost osim NO ili YES.
*/
--13 bodova
ALTER TABLE Prodaja
DROP CONSTRAINT CK_OnlineNarudzba

ALTER TABLE Prodaja
ALTER COLUMN Online_Narudzba NVARCHAR(3)

ALTER PROCEDURE proc_prodaja
AS
BEGIN
UPDATE Prodaja
SET Online_Narudzba = 'NO'
WHERE Online_Narudzba = '0'
UPDATE Prodaja
SET Online_Narudzba = 'YES'
WHERE Online_Narudzba = '1'
END

EXEC proc_prodaja

select * from Prodaja

ALTER TABLE Prodaja
ADD CONSTRAINT CK_Online_Narudzba CHECK(Online_Narudzba = 'YES' OR Online_Narudzba = 'NO')

------------------------------------------
--9.
/*
a) 
Nad kolonom god_rod tabele radnik kreirati ograničenje kojim će
se onemogućiti unos bilo koje godine iz budućnosti kao godina rođenja.
Testirati funkcionalnost kreiranog ograničenja navođenjem 
koda za insert podataka kojim će se kao godina rođenja
pokušati unijeti bilo koja godina iz budućnosti.
*/
ALTER TABLE Radnik
ADD CONSTRAINT CK_God_Rod CHECK(God_Rod <= YEAR(GETDATE()))

INSERT INTO Radnik
VALUES(20001, 'Njemacka', 'DNADNAKOK', 2022, 'M', 'III' )

SELECT * FROM Radnik

/*
b) Nad kolonom drzavaID tabele radnik kreirati ograničenje kojim će se ograničiti dužina podatka na 7 znakova. 
Ako je prethodno potrebno, izvršiti prilagodbu kolone, pri čemu nije dozvoljeno prilagođavati podatke čiji 
dužina iznosi 7 ili manje znakova.
Testirati funkcionalnost kreiranog ograničenja navođenjem koda za insert podataka 
kojim će se u drzavaID pokušati unijeti podataka duži od 7 znakova bilo koja godina iz budućnosti.
*/
--10 bodova
UPDATE Radnik
SET DrzavaID=LEFT(DrzavaID, 7)

ALTER TABLE Radnik
ADD CONSTRAINT CK_DrzavaID check (LEN(DrzavaID) <= 7)

INSERT INTO Radnik
VALUES(1171111, '1234567', 'LOGINID', 1999, 'M', 'I')

SELECT * FROM Radnik
-----------------------------------------------
--10.
/*
Kreirati backup baze na default lokaciju, obrisati bazu, a zatim izvršiti restore baze. 
Uslov prihvatanja koda je da se može izvršiti.
*/
--2 boda

BACKUP DATABASE JUL20
TO DISK = 'C:\Program Files\Microsoft SQL Server\MSSQL14.MSSQLSERVER_OLAP\MSSQL\Backup\JUL20'

USE master

DROP DATABASE JUL20

RESTORE DATABASE JUL20
FROM DISK = 'C:\Program Files\Microsoft SQL Server\MSSQL14.MSSQLSERVER_OLAP\MSSQL\Backup\JUL20'
