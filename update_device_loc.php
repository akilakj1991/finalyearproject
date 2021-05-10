<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "mysql";

$device_id=$_GET["device_id"];
$lat=$_GET["lat"];
$lng=$_GET["lng"];
$time_date=$_GET["time_date"];

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "INSERT INTO device_location (device_id, lat, lng, time_date)
VALUES ('$device_id', '$lat', '$lng', now());";

if ($conn->multi_query($sql) === TRUE) {
	$last_id = $conn->insert_id;
    //echo "New records created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$sql = "SELECT * FROM device_location where id=" . $last_id ;
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo "id " . $row["id"]. "-lat" . $row["lat"]. "-long" . $row["lng"]. "-timestamp" . $row["time_date"]. "<br>";
    }
} else {
    echo "0 results";
}

$conn->close();
?>