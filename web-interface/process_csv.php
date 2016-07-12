<?php

    $lr_exec = '/home/varun/programming/ML/linear_regression/linear_regression_driver/lr_run_web';
    $refine = $_POST['refine'];
    $uid = $_POST['uid'];

    /* Fill the following in the $refine if/else block */
    $input_points_path = "";
    $input_weights_path = "";
    $learning_rate = "";
    $epoch = "";
    $output_prefix = "";

    /* If its not a refine run ! do all 
     * the pre-processing
     */
    if($refine == "no") {
        $upload_file_location = 'uploads/' . $uid . '.csv';

        /* Check if the uploaded file exists */
        if(!file_exists($upload_file_location)) {
            echo "File not found";
            return;
        }

        $result_store_dir = 'Results/' . $uid . '_run/';
        $target_file_location = $result_store_dir . '/' . $uid . '.csv';

        /* Make a directory in Results to store the results
         * for this run
         */
        if(!mkdir($result_store_dir)) {
            echo "Cannot create result store";
            return;
        }

        /* Copy the uploaded file to result store dir for processing */
        if(!copy($upload_file_location, $target_file_location)) {
            echo "Cannot copy file";
            return;
        }

        $input_points_path = $target_file_location;
        $input_weights_path = "0";
        $learning_rate = $_POST['learning_rate'];
        $epoch = $_POST['epoch'];
        $output_prefix = $_POST['prefix'];
    } else {
        /* Check for weights and input files */
        $result_dir = 'Results/' . $uid . '_run/';

        $input_points_path = $result_dir . '/' . $uid . '.csv';
        $input_weights_path = $result_dir . '/' . $uid . '_lr_dump.txt';
        $learning_rate = $_POST['learning_rate'];
        $epoch = $_POST['epoch'];
        $output_prefix = $_POST['prefix'];

        /* Make sure that both inputs and weights file exists */
        if(!file_exists($input_points_path)) {
            echo "Input file does not exits";
            return;
        }

        if(!file_exists($input_weights_path)) {
                echo "Input weights does not exist";
                return;
        }
    }
?>