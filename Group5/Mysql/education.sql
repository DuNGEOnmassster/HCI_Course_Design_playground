-- MySQL dump 10.13  Distrib 8.0.30, for Win64 (x86_64)
--
-- Host: localhost    Database: education
-- ------------------------------------------------------
-- Server version	8.0.28

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `administrator`
--

DROP TABLE IF EXISTS `administrator`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `administrator` (
  `admin_id` char(10) NOT NULL,
  `password` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`admin_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `administrator`
--

LOCK TABLES `administrator` WRITE;
/*!40000 ALTER TABLE `administrator` DISABLE KEYS */;
INSERT INTO `administrator` VALUES ('20','123456');
/*!40000 ALTER TABLE `administrator` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `course`
--

DROP TABLE IF EXISTS `course`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `course` (
  `course_id` char(4) NOT NULL,
  `course_name` varchar(255) NOT NULL,
  `course_hour` int DEFAULT NULL,
  `course_type` varchar(255) DEFAULT NULL,
  `course_credit` float DEFAULT NULL,
  PRIMARY KEY (`course_id`),
  KEY `c_name` (`course_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `course`
--

LOCK TABLES `course` WRITE;
/*!40000 ALTER TABLE `course` DISABLE KEYS */;
INSERT INTO `course` VALUES ('0101','数据库',48,'必修',4),('0102','操作系统',48,'必修',4),('0103','计算机网络',48,'必修',4),('0201','智能系统',48,'必修',4.5),('0202','AI导论',36,'必修',3.5),('0203','AI设计',48,'必修',4);
/*!40000 ALTER TABLE `course` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `department`
--

DROP TABLE IF EXISTS `department`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `department` (
  `dept_id` char(4) NOT NULL,
  `dept_name` varchar(255) DEFAULT NULL,
  `dept_master` char(10) DEFAULT NULL,
  PRIMARY KEY (`dept_id`),
  UNIQUE KEY `dept_name_UNIQUE` (`dept_name`),
  KEY `f7` (`dept_master`),
  KEY `d_name` (`dept_name`),
  CONSTRAINT `f7` FOREIGN KEY (`dept_master`) REFERENCES `teacher` (`tch_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `department`
--

LOCK TABLES `department` WRITE;
/*!40000 ALTER TABLE `department` DISABLE KEYS */;
INSERT INTO `department` VALUES ('01','CS','10001'),('02','AI','20003'),('03','IC','20002'),('04','IS',NULL);
/*!40000 ALTER TABLE `department` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `func`
--

DROP TABLE IF EXISTS `func`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `func` (
  `f_name` varchar(45) NOT NULL,
  `f_state` int DEFAULT NULL,
  PRIMARY KEY (`f_name`),
  CONSTRAINT `ck_state` CHECK ((`f_state` in (0,1)))
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `func`
--

LOCK TABLES `func` WRITE;
/*!40000 ALTER TABLE `func` DISABLE KEYS */;
INSERT INTO `func` VALUES ('course_select',0);
/*!40000 ALTER TABLE `func` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `selection`
--

DROP TABLE IF EXISTS `selection`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `selection` (
  `stu_id` char(10) NOT NULL,
  `course_id` char(4) NOT NULL,
  `score` float DEFAULT NULL,
  PRIMARY KEY (`stu_id`,`course_id`),
  KEY `f2` (`course_id`),
  CONSTRAINT `f1` FOREIGN KEY (`stu_id`) REFERENCES `student` (`stu_id`),
  CONSTRAINT `f2` FOREIGN KEY (`course_id`) REFERENCES `course` (`course_id`),
  CONSTRAINT `ck_score` CHECK ((`score` between 0 and 100))
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `selection`
--

LOCK TABLES `selection` WRITE;
/*!40000 ALTER TABLE `selection` DISABLE KEYS */;
INSERT INTO `selection` VALUES ('2020303001','0101',0),('2020303001','0102',0),('2020303001','0103',0),('2020303001','0201',0);
/*!40000 ALTER TABLE `selection` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `student`
--

DROP TABLE IF EXISTS `student`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `student` (
  `stu_id` char(10) NOT NULL,
  `stu_name` varchar(255) NOT NULL,
  `stu_gen` char(1) DEFAULT NULL,
  `stu_age` int DEFAULT NULL,
  `stu_dept` char(4) NOT NULL,
  `stu_passwd` varchar(255) DEFAULT NULL,
  `is_check` char(1) DEFAULT 'N',
  PRIMARY KEY (`stu_id`),
  KEY `f5` (`stu_dept`),
  KEY `s_name` (`stu_name`),
  CONSTRAINT `f5` FOREIGN KEY (`stu_dept`) REFERENCES `department` (`dept_name`),
  CONSTRAINT `ck_check` CHECK ((`is_check` in (_utf8mb4'Y',_utf8mb4'N'))),
  CONSTRAINT `ck_sgender` CHECK ((`stu_gen` in (_utf8mb4'M',_utf8mb4'F')))
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `student`
--

LOCK TABLES `student` WRITE;
/*!40000 ALTER TABLE `student` DISABLE KEYS */;
INSERT INTO `student` VALUES ('2020303001','Huang','M',20,'CS','123','Y'),('2020303002','Li','M',20,'CS','123','N'),('2020303003','Gao','M',20,'CS','123','N');
/*!40000 ALTER TABLE `student` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `teacher`
--

DROP TABLE IF EXISTS `teacher`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `teacher` (
  `tch_id` char(10) NOT NULL,
  `tch_name` varchar(255) NOT NULL,
  `tch_gen` char(1) DEFAULT NULL,
  `tch_dept` char(4) NOT NULL,
  `tch_title` varchar(255) DEFAULT NULL,
  `tch_age` int DEFAULT NULL,
  `tch_birth` date DEFAULT NULL,
  `tch_salary` float DEFAULT NULL,
  `tch_passwd` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`tch_id`),
  KEY `f6` (`tch_dept`),
  KEY `t_name` (`tch_name`),
  CONSTRAINT `f6` FOREIGN KEY (`tch_dept`) REFERENCES `department` (`dept_name`),
  CONSTRAINT `ck_tgender` CHECK ((`tch_gen` in (_utf8mb4'M',_utf8mb4'F')))
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `teacher`
--

LOCK TABLES `teacher` WRITE;
/*!40000 ALTER TABLE `teacher` DISABLE KEYS */;
INSERT INTO `teacher` VALUES ('10001','Wang','M','CS','教授',46,'1976-01-01',30000,'123'),('10002','Li','M','CS','教授',42,'1980-01-01',25000,'123'),('10003','Huang','M','CS','讲师',32,'1990-01-01',18000,'123'),('20001','Sun','M','AI','教授',42,'1980-01-01',26000,'123'),('20002','Zhao','M','AI','讲师',32,'1980-01-01',20000,'123'),('20003','Liu','M','AI','教授',48,'1974-01-01',30000,'123');
/*!40000 ALTER TABLE `teacher` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `teaching`
--

DROP TABLE IF EXISTS `teaching`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `teaching` (
  `tch_id` char(10) NOT NULL,
  `course_id` char(4) NOT NULL,
  `since` date DEFAULT NULL,
  PRIMARY KEY (`tch_id`,`course_id`),
  KEY `f4` (`course_id`),
  CONSTRAINT `f3` FOREIGN KEY (`tch_id`) REFERENCES `teacher` (`tch_id`),
  CONSTRAINT `f4` FOREIGN KEY (`course_id`) REFERENCES `course` (`course_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `teaching`
--

LOCK TABLES `teaching` WRITE;
/*!40000 ALTER TABLE `teaching` DISABLE KEYS */;
INSERT INTO `teaching` VALUES ('10002','0102','2022-09-08'),('10003','0103','2022-09-06'),('20003','0203','2022-10-08');
/*!40000 ALTER TABLE `teaching` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-12-18 13:52:00
