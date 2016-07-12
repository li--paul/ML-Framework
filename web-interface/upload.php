<?php

    /*
     * The script echos the following error codes
     * 000 - Input file upload success && Weights file upload success (Continue processing)
     * 1XX - Input file upload failed && Weights file upload not attempted (Abort)
     * 01X - Input file upload success && Weights file does not make sense (Continue)
     * 001 - Input file upload success && Weights file not uploaded (Abort)
     */
    
    session_start();
    ini_set('file_uploads', 'On');
    ini_set('upload_max_filesize', '1000M');
    ini_set('post_max_size', '1500M');

    /* Upload input file */
    $upload_file = basename($_FILES["fileinput_input"]["name"]);
    $upload_file_parts = explode('.', $upload_file);
    if(end($upload_file_parts) != "csv") {
        echo $upload_file . "Not a csv file";
        return;
    }

    $uniq_id = $_POST['uniq_id'];
    $target_dir = "uploads/";
    $target_file = $target_dir . $uniq_id . '.csv'; 

    if (move_uploaded_file($_FILES["fileinput_input"]["tmp_name"], $target_file)) {
        echo "Upload success.";
    } else {
        echo "Upload failed.";
    }

    session_destroy();
?>
