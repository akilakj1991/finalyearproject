<?php
require("phpsqlajax_dbinfo.php");

function parseToXML($htmlStr) 
{ 
$xmlStr=str_replace('<','&lt;',$htmlStr); 
$xmlStr=str_replace('>','&gt;',$xmlStr); 
$xmlStr=str_replace('"','&quot;',$xmlStr); 
$xmlStr=str_replace("'",'&#39;',$xmlStr); 
$xmlStr=str_replace("&",'&amp;',$xmlStr); 
return $xmlStr; 
} 
// Create connection
$conn = new mysqli($servername, $username, $password, $database);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
 
/* // Opens a connection to a MySQL server
$connection=mysql_connect ("localhost", $username, $password);
if (!$connection) {
  die('Not connected : ' . mysql_error());
} 

// Set the active MySQL database
$db_selected = mysql_select_db($database, $connection);
if (!$db_selected) {
  die ('Can\'t use db : ' . mysql_error());
} */

// Select all the rows in the markers table
$sql= "SELECT * FROM device_location WHERE ID=166";

$result = $conn->query($sql);

if ($result->num_rows > 0) {
	header("Content-type: text/xml");

// Start XML file, echo parent node
echo '<markers>';

// Iterate through the rows, printing XML nodes for each
while ($row = $result->fetch_assoc()){
  // ADD TO XML DOCUMENT NODE
  echo '<marker ';
  // echo 'name="' . parseToXML($row['name']) . '" ';
  // echo 'address="' . parseToXML($row['address']) . '" ';
  echo 'name="" ';
  echo 'address="" ';
  echo 'lat="' . $row['lat'] . '" ';
  echo 'lng="' . $row['lng'] . '" ';
  echo 'type="' . $row['time_date'] . '" ';
  echo '/>';
} 

// End XML file
echo '</markers>';
    
} else {
    echo "0 results";
}

$conn->close();

/* $result = mysql_query($query);
if (!$result) {
  die('Invalid query: ' . mysql_error());
}
 */


?>