<?php
require_once("config.php");  // Pastikan konfigurasi koneksi database sudah benar

// Query untuk mendapatkan data suhu maksimum dan minimum
$query_suhu = "SELECT MAX(suhu) as suhumax, MIN(suhu) as suhumin, AVG(suhu) as average FROM tb_cuaca";
$result_suhu = $conn->query($query_suhu);
$suhu_data = $result_suhu->fetch_assoc();

// Query untuk mendapatkan data suhu maksimum dan kelembaban maksimum
$query_max_humid = "
    SELECT * 
    FROM tb_cuaca 
    WHERE suhu = (SELECT MAX(suhu) FROM tb_cuaca) 
      AND humid = (SELECT MAX(humid) FROM tb_cuaca)";
$result_max_humid = $conn->query($query_max_humid);
$max_humid_data = [];
while ($row = $result_max_humid->fetch_assoc()) {
    $max_humid_data[] = [
        "id" => $row['id'],
        "suhu" => $row['suhu'],
        "humid" => $row['humid'],
        "Lux" => $row['lux'],
        "Timestamp" => $row['ts']
    ];
}

// Query untuk mendapatkan data bulan-tahun maksimum
$query_month_year = "
    SELECT DATE_FORMAT(ts, '%m-%Y') as month_year 
    FROM tb_cuaca 
    GROUP BY month_year 
    ORDER BY MAX(suhu) DESC";
$result_month_year = $conn->query($query_month_year);
$month_year_data = [];
while ($row = $result_month_year->fetch_assoc()) {
    $month_year_data[] = [
        "month_year" => $row['month_year']
    ];
}

// Membentuk struktur data
$response = [
    "suhumax" => $suhu_data['suhumax'],
    "suhumin" => $suhu_data['suhumin'],
    "average" => number_format($suhu_data['average'], 2),
    "nilai_suhu_max_humid_max" => $max_humid_data,
    "month_year_max" => $month_year_data
];

// Mengembalikan hasil dalam format JSON
header('Content-Type: application/json');
echo json_encode($response, JSON_PRETTY_PRINT);

// Menutup koneksi
$conn->close();
?>
