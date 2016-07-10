<?php
    session_start();
    ini_set('file_uploads', 'On');
    ini_set('upload_max_filesize', '1000M');
    ini_set('post_max_size', '1500M');

    $target_dir = "uploads/";
    $target_file = $target_dir . basename($_FILES["fileinput"]["name"]);

    if (move_uploaded_file($_FILES["fileinput"]["tmp_name"], $target_file)) {
        echo "Upload success.";
    } else {
        echo "Sorry, there was an error uploading your file.";
    }

    session_destroy();
?>
