----------------------------
--1.
----------------------------
/*
Kreirati bazu pod vlastitim brojem indeksa
*/
CREATE DATABASE JUN2020
USE JUN2020

-----------------------------------------------------------------------
--Prilikom kreiranja tabela voditi računa o njihovom međusobnom odnosu.
-----------------------------------------------------------------------
/*
a) 
Kreirati tabelu dobavljac sljedeće strukture:
	- dobavljac_id - cjelobrojna vrijednost, primarni ključ
	- dobavljac_br_rac - 50 unicode karaktera
	- naziv_dobavljaca - 50 unicode karaktera
	- kred_rejting - cjelobrojna vrijednost
*/
CREATE TABLE Dobavljac(
DobavljacID INT CONSTRAINT PK_DobavljacID PRIMARY KEY,
Dobavljac_br_rac NVARCHAR(50),
Naziv_Dobavljaca NVARCHAR (50),
Kred_rejting INT
)

/*
b)
Kreirati tabelu narudzba sljedeće strukture:
	- narudzba_id - cjelobrojna vrijednost, primarni ključ
	- narudzba_detalj_id - cjelobrojna vrijednost, primarni ključ
	- dobavljac_id - cjelobrojna vrijednost
	- dtm_narudzbe - datumska vrijednost
	- naruc_kolicina - cjelobrojna vrijednost
	- cijena_proizvoda - novčana vrijednost
*/
CREATE TABLE Narudzba(
NarudzbaID INT,
Narudzba_Detalj_Id INT,
Dobavljac_id INT,
Dtm_Narudzbe DATE,
Naruc_kolicina INT,
Cijena_Proizvoda MONEY,
CONSTRAINT PK_Narudzba PRIMARY KEY(NarudzbaID, Narudzba_Detalj_Id),
CONSTRAINT FK_Dobavljac FOREIGN KEY(Dobavljac_id) REFERENCES Dobavljac(DobavljacID)
)
/*
c)
Kreirati tabelu dobavljac_proizvod sljedeće strukture:
	- proizvod_id cjelobrojna vrijednost, primarni ključ
	- dobavljac_id cjelobrojna vrijednost, primarni ključ
	- proiz_naziv 50 unicode karaktera
	- serij_oznaka_proiz 50 unicode karaktera
	- razlika_min_max cjelobrojna vrijednost
	- razlika_max_narudzba cjelobrojna vrijednost
*/
--10 bodova
CREATE TABLE Dobavljac_Proizvod(
ProizvodID INT,
Dobavljac_id INT,
Proiz_Naziv NVARCHAR(50),
Serij_Oznaka_Proiz NVARCHAR(50),
Razlika_min_max INT,
Razlika_max_narudzba INT,
CONSTRAINT PK_Dobavljac_Proizvod PRIMARY KEY(ProizvodID, Dobavljac_id),
CONSTRAINT FK_Dobavljac_Pr FOREIGN KEY(Dobavljac_id) REFERENCES Dobavljac(DobavljacID)
)


----------------------------
--2. Insert podataka
----------------------------
/*
a) 
U tabelu dobavljac izvršiti insert podataka iz tabele Purchasing.Vendor prema sljedećoj strukturi:
	BusinessEntityID -> dobavljac_id 
	AccountNumber -> dobavljac_br_rac 
	Name -> naziv_dobavljaca
	CreditRating -> kred_rejting
*/
INSERT INTO Dobavljac
SELECT V.BusinessEntityID, V.AccountNumber, V.Name, V.CreditRating
FROM AdventureWorks2017.Purchasing.Vendor AS V

/*
b) 
U tabelu narudzba izvršiti insert podataka iz tabela Purchasing.PurchaseOrderHeader i Purchasing.
PurchaseOrderDetail prema sljedećoj strukturi:
	PurchaseOrderID -> narudzba_id
	PurchaseOrderDetailID -> narudzba_detalj_id
	VendorID -> dobavljac_id 
	OrderDate -> dtm_narudzbe 
	OrderQty -> naruc_kolicina 
	UnitPrice -> cijena_proizvoda
*/
INSERT INTO Narudzba
SELECT POH.PurchaseOrderID, POD.PurchaseOrderDetailID, POH.VendorID, POH.OrderDate, POD.OrderQty, POD.UnitPrice
FROM AdventureWorks2017.Purchasing.PurchaseOrderHeader AS POH INNER JOIN AdventureWorks2017.Purchasing.PurchaseOrderDetail AS POD ON
POD.PurchaseOrderID = POH.PurchaseOrderID

/*
c) 
U tabelu dobavljac_proizvod izvršiti insert podataka iz tabela Purchasing.ProductVendor i 
Production.Product prema sljedećoj strukturi:
	ProductID -> proizvod_id 
	BusinessEntityID -> dobavljac_id 
	Name -> proiz_naziv 
	ProductNumber -> serij_oznaka_proiz
	MaxOrderQty - MinOrderQty -> razlika_min_max 
	MaxOrderQty - OnOrderQty -> razlika_max_narudzba
uz uslov da se povuku samo oni zapisi u kojima ProductSubcategoryID nije NULL vrijednost.
*/
--10 bodova
INSERT INTO Dobavljac_Proizvod
SELECT PP.ProductID, PV.BusinessEntityID, PP.Name, PP.ProductNumber, PV.MaxOrderQty - PV.MinOrderQty AS razlika_min_max, PV.MaxOrderQty - PV.OnOrderQty AS razlika_max_narudzba
FROM AdventureWorks2017.Purchasing.ProductVendor AS PV INNER JOIN AdventureWorks2017.Production.Product AS PP ON PP.ProductID = PV.ProductID
----------------------------
--3.
----------------------------
/*
Koristeći sve tri tabele iz vlastite baze kreirati pogled view_dob_god sljedeće strukture:
	- dobavljac_id
	- proizvod_id
	- naruc_kolicina
	- cijena_proizvoda
	- ukupno, kao proizvod naručene količine i cijene proizvoda
Uslov je da se dohvate samo oni zapisi u kojima je narudžba obavljena 2013. ili 2014. godine i da se 
broj računa dobavljača završava cifrom 1.
*/
--10 bodova
CREATE VIEW view_dob_god
AS
SELECT D.DobavljacID, DP.ProizvodID, N.Naruc_kolicina, N.Cijena_Proizvoda, (N.Naruc_kolicina * N.Cijena_Proizvoda) AS Ukupno
FROM Narudzba AS N INNER JOIN Dobavljac AS D ON D.DobavljacID = N.Dobavljac_id INNER JOIN Dobavljac_Proizvod AS DP ON DP.Dobavljac_id = D.DobavljacID
WHERE (YEAR(N.Dtm_Narudzbe) = 2013 OR YEAR(N.Dtm_Narudzbe) = 2014) AND RIGHT(D.Dobavljac_br_rac, 1) LIKE '1'


SELECT * FROM Narudzba
----------------------------
--4.
----------------------------
/*
Koristeći pogled view_dob_god kreirati proceduru proc_dob_god koja će sadržavati parametar naruc_kolicina i imati sljedeću strukturu:
	- dobavljac_id
	- proizvod_id
	- suma_ukupno, sumirana vrijednost kolone ukupno po dobavljac_id i proizvod_id
Uslov je da se dohvataju samo oni zapisi u kojima je naručena količina trocifreni broj.
Nakon kreiranja pokrenuti proceduru za vrijednost naručene količine 300.
*/
--10 bodova
CREATE PROCEDURE proc_dob_god
(
@naruc_kolicina INT
)
AS
BEGIN
SELECT DobavljacID, ProizvodID, SUM(Ukupno) AS Suma_Ukupno
FROM view_dob_god
WHERE LEN(Naruc_kolicina) = 3 AND Naruc_kolicina = @naruc_kolicina
GROUP BY DobavljacID, ProizvodID
END

EXEC proc_dob_god 300

----------------------------
--5.
----------------------------
/*
a)
Tabelu dobavljac_proizvod kopirati u tabelu dobavljac_proizvod_nova.
b) 
Iz tabele dobavljac_proizvod_nova izbrisati kolonu razlika_min_max.
c)
U tabeli dobavljac_proizvod_nova kreirati novu kolonu razlika. Kolonu popuniti razlikom vrijednosti kolone 
razlika_max_narudzba i srednje vrijednosti ove kolone, uz uslov da ako se u zapisu nalazi NULL vrijednost 
u kolonu razlika smjestiti 0.
*/
--15 bodova
SELECT * INTO dobavljac_proizvod_nova FROM Dobavljac_Proizvod

ALTER TABLE dobavljac_proizvod_nova
DROP COLUMN Razlika_min_max

ALTER TABLE dobavljac_proizvod_nova
ADD Razlika FLOAT

UPDATE dobavljac_proizvod_nova
SET Razlika = ISNULL(Razlika_max_narudzba - (SELECT AVG(Razlika_max_narudzba) FROM dobavljac_proizvod_nova), 0)
----------------------------
--6.
----------------------------
/*
Prebrojati koliko u tabeli dobavljac_proizvod ima različitih serijskih oznaka proizvoda koje završavaju bilo kojim slovom engleskog alfabeta, 
a koliko ima onih koji ne završavaju bilo kojim slovom engleskog alfabeta. Upit treba da vrati poruke:
	'Različitih serijskih oznaka proizvoda koje završavaju slovom engleskog alfabeta ima:' iza čega slijedi broj zapisa 
	i
	'Različitih serijskih oznaka proizvoda koje NE završavaju slovom engleskog alfabeta ima:' iza čega slijedi broj zapisa
*/
--10 bodova
SELECT 'Različitih serijskih oznaka proizvoda koje završavaju slovom engleskog alfabeta ima: ' + 
CONVERT(NVARCHAR, COUNT(DISTINCT(Serij_Oznaka_Proiz))) AS Informacija
FROM Dobavljac_Proizvod
WHERE ISNUMERIC(RIGHT(Serij_Oznaka_Proiz, 1)) = 0
UNION 
SELECT 'Različitih serijskih oznaka proizvoda koje NE završavaju slovom engleskog alfabeta ima: ' + 
CONVERT(NVARCHAR, COUNT(DISTINCT(Serij_Oznaka_Proiz))) AS Informacija
FROM Dobavljac_Proizvod
WHERE ISNUMERIC(RIGHT(Serij_Oznaka_Proiz, 1)) = 1

----------------------------
--7.
----------------------------
/*
a)
Dati informaciju o dužinama podatka u koloni serij_oznaka_proiz tabele dobavljac_proizvod. 
b)
Dati informaciju o broju različitih dužina podataka u koloni serij_oznaka_proiz tabele dobavljac_proizvod. 
Poruka treba biti u obliku: 'Kolona serij_oznaka_proiz ima ___ različite dužinr podataka.
' Na mjestu donje crte se nalazi izračunati brojčani podatak.
*/
--10 bodova
SELECT LEN(Serij_Oznaka_Proiz) AS Duzina 
FROM Dobavljac_Proizvod

SELECT LEN(Serij_Oznaka_Proiz) AS Duzina, COUNT(LEN(Serij_Oznaka_Proiz)) AS Prebrojano
FROM Dobavljac_Proizvod
GROUP BY LEN(Serij_Oznaka_Proiz)

SELECT 'Kolona serij_oznaka_proiz ima ' + CONVERT(NVARCHAR,COUNT(DISTINCT(LEN(Serij_Oznaka_Proiz)))) + ' različite dužine podataka.' AS Informacija
FROM Dobavljac_Proizvod

----------------------------
--8.
----------------------------
/*
Prebrojati kod kolikog broja dobavljača je broj računa kreiran korištenjem više od jedne riječi iz naziva dobavljača. 
Jednom riječi se podrazumijeva skup slova koji nije prekinut blank (space) znakom. 
*/
--10 bodova
select COUNT(*) AS Prebrojano
FROM Dobavljac 
WHERE LEN(SUBSTRING(Dobavljac_br_rac, 0, CHARINDEX('0', Dobavljac_br_rac))) > LEN(SUBSTRING(Naziv_Dobavljaca, 0, CHARINDEX(' ', Naziv_Dobavljaca)))
AND  CHARINDEX(' ', Naziv_Dobavljaca) != 0

-- prebroji slova tj string od dobavljac_br_rac i ako je on > od prve rijeci naziva dobavljaca i ako ima razmak u nazivu dobavljaca, 
--tj. ako je sastavljeno od vise rijeci, counta ih


----------------------------
--9.
----------------------------
/*
Koristeći pogled view_dob_god kreirati proceduru proc_djeljivi koja će sadržavati parametar prebrojano i kojom će se prebrojati 
broj pojavljivanja vrijednosti u koloni naruc_kolicina koje su djeljive sa 100. Sortirati po koloni prebrojano. 
Nakon kreiranja pokrenuti proceduru za sljedeću vrijednost parametra prebrojano = 10
*/
--13 bodova
CREATE VIEW view_prebrojano
AS
SELECT Naruc_kolicina, COUNT(Naruc_kolicina) AS Prebrojano
FROM view_dob_god
WHERE Naruc_kolicina % 100 = 0
GROUP BY Naruc_kolicina

CREATE PROCEDURE proc_djeljivi
(
@Prebrojano INT
)
AS
BEGIN
SELECT *
FROM view_prebrojano
WHERE @Prebrojano = Prebrojano
ORDER BY Prebrojano
END
EXEC proc_djeljivi 10
----------------------------
--10.
----------------------------
/*
a) Kreirati backup baze na default lokaciju.
b) Napisati kod kojim će biti moguće obrisati bazu.
c) Izvršiti restore baze.
Uslov prihvatanja kodova je da se mogu pokrenuti.
*/
--2 boda
BACKUP DATABASE JUL2020
TO DISK = 'C:\Program Files\Microsoft SQL Server\MSSQL14.MSSQLSERVER_OLAP\MSSQL\Backup\JUN2020'

USE master

DROP DATABASE JUN2020

RESTORE DATABASE JUN2020
FROM DISK = 'C:\Program Files\Microsoft SQL Server\MSSQL14.MSSQLSERVER_OLAP\MSSQL\Backup\JUN2020'
