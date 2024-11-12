<?php
$host = "localhost";         // Nama host server database
$username = "root";          // Username MySQL (biasanya "root" untuk localhost)
$password = "";              // Password MySQL (kosong untuk localhost)
$database = "tb_cuaca"; // Nama database Anda

// Membuat koneksi
$conn = new mysqli($host, $username, $password, $database);

// Periksa koneksi
if ($conn->connect_error) {
    die("Koneksi gagal: " . $conn->connect_error);
}
?>
