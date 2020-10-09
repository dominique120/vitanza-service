-- Table Districts --------------------------
-- 
CREATE TABLE Districts
(
    `DistrictId` int 		NOT NULL PRIMARY KEY AUTO_INCREMENT,
    `DistrictName` varchar(20)		NOT NULL
);


-- Table Order --------------------------
-- 
CREATE TABLE Orders
(
    `OrderId` int 		NOT NULL PRIMARY KEY AUTO_INCREMENT,
    `ClientId` int 		NOT NULL,
    `DatePlaced` datetime		NOT NULL,
    `Notes` varchar(500) 		NOT NULL,
    `Delivered` bool 		NOT NULL DEFAULT false,
    `DateDelivered` datetime		NOT NULL
);


-- Table OrderDetail --------------------------
-- 
CREATE TABLE OrderDetails
(
    `OrderDetailId` int 		NOT NULL PRIMARY KEY AUTO_INCREMENT,
    `OrderId` int 		NOT NULL,
    `Product` int 		NOT NULL,
    `Quantity` int 		NOT NULL,
    `OverridePrice` float 		NOT NULL
);


-- Table Client --------------------------
-- 
CREATE TABLE Clients
(
    `ClientId` int 		NOT NULL PRIMARY KEY AUTO_INCREMENT ,
    `FisrtName` varchar(100) 		NOT NULL,
    `LastNames` varchar(100)		NOT NULL,
    `PrimaryAddress` varchar(100)		NOT NULL,
    `SecondaryAddress` varchar(100)		NOT NULL,
    `PrimaryPhone` varchar(100)		NOT NULL,
    `SecondaryPhone` varchar(100) 		NOT NULL,
    `District` int 		NOT NULL
);


-- Table Product --------------------------
-- 
CREATE TABLE Products
(
    `ProductId` int 		NOT NULL PRIMARY KEY AUTO_INCREMENT ,
    `ProductName` varchar 		(30)NOT NULL,
    `ProductDescription` varchar 		(100)NOT NULL,
    `Price` Double 		NOT NULL,
    `AvailableStock` int 		NOT NULL
);




-- References for OrderDetail --------------------------
ALTER TABLE `OrderDetails`
ADD FOREIGN KEY (OrderId) REFERENCES Orders (OrderId);
 

ALTER TABLE `OrderDetails`
ADD FOREIGN KEY (Product) REFERENCES Products (ProductId);
 


-- References for Client --------------------------
ALTER TABLE `Clients`
ADD  FOREIGN KEY (District) REFERENCES Districts (DistrictId);
 

ALTER TABLE `Orders`
ADD  FOREIGN KEY (ClientId) REFERENCES Clients (ClientId);
 



