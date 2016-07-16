<?php

    /*
     * Return values of the PHP.
     * The PHP file echo's a string of 3 characters.
     * Return '000' for success.
     * Return '1XX' if failed to upload input file.
     * Return '01X' if failed to upload weights file.
     * Return '001' if failed to create execution dir.
     * Return '002' if failed to copy input file to exec. dir.
     * Return '003' if failed to copy weights file to exec. dir.
     */

    function upload_file($upload_path, $target_path) {
        if (move_uploaded_file($upload_path, $target_path)) {
            return true; // upload success
        } else {
            return false; // upload failed
        }
    }
    
    /*
     * Return "0" on success.
     * Return error code "1" if unable to create dir.
     * Return error code "2" if unable to copy input file.
     * Return error code "3" if unable to copy weights file.
     */ 
    function setup_execution_directory($uid, $input_filepath, $weights_filepath) {
        $result_dir = 'Results/' . $uid . '_run/';
        /* Create results directory */
        if(!mkdir($result_dir, 0777)) {
            return "1";
        }

        /* The 0 in the file name denotes that this 
         * is the initial input and weights file;
         * 'Refine' button will create new input and
         * weights file
         */ 
        $target_input_file_location = $result_dir . '/' . $uid . '_input_0.csv';
        $target_weights_file_location = $result_dir . '/' . $uid . '_weights_0.csv';

        /* Copy the uploaded file to result store dir for processing */
        if(!copy($input_filepath, $target_input_file_location)) {
            return "2";
        }

        /* Copy the weights file to result store dir for processing */
        if($weights_filepath != "0") { 
            if(!copy($weights_filepath, $target_weights_file_location)) {
                return "3";
            }
        }

        return "0";
    }

    session_start();
    ini_set('file_uploads', 'On');
    ini_set('upload_max_filesize', '1000M');
    ini_set('post_max_size', '1500M');

    $input_upload_status = false;
    $weights_upload_status = false;

    $uniq_id = $_POST['uniq_id'];

    /* Upload input file */
    $input_upload_status = upload_file($_FILES['fileinput_input']['tmp_name'], 'uploads/' . $uniq_id . '_input.csv');

    /* Upload weights filename */
    $validated_weights_filename = $_POST['validated_weights_filename'];
    if($validated_weights_filename == "0") {
        /* Nothing to upload */
        $weights_upload_status = true;
    } else {
        /* Something to upload */
        $weights_upload_status = upload_file($_FILES['fileinput_weights']['tmp_name'], 'uploads/' . $uniq_id . '_weights.csv');
    }

    if($input_upload_status && $weights_upload_status) {
        if($validated_weights_filename == "0") {
            echo "00" . setup_execution_directory($uniq_id, 'uploads/' . $uniq_id . '_input.csv', "0");
        } else {
            echo "00" . setup_execution_directory($uniq_id, 'uploads/' . $uniq_id . '_input.csv', 'uploads/' . $uniq_id . '_weights.csv');
        }
    } else {
        if(!$input_upload_status) { echo "1XX"; }
        else { echo "01X"; }
    }

    session_destroy();
?>
