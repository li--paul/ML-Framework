<?php

    /*
     * PHP file returns '0' on success.
     */

    $lr_exec ='/home/varun/programming/ML/linear_regression/linear_regression_driver/lr_run_web';

    $uid = $_POST['uid'];
    $result_dir = 'Results/' . $uid . '_run/';
    if(!chdir($result_dir)) {
        echo "Cannot change to " . $result_dir;
        return;
    } 

    $input_points_path = $uid . '_input.csv';
    $input_weights_path = $uid . '_weights.csv';
    $learning_rate = $_POST['learning_rate'];
    $epoch = $_POST['epoch'];
    $output_prefix = $uid;

    /* If weights file not available; Initialize with random weights */
    if(!file_exists($input_weights_path)) {
        $input_weights_path = '0';
    }

    shell_exec( 'tsp ' . $lr_exec . ' ' . $input_points_path . ' ' . $input_weights_path . ' ' . $learning_rate . ' ' . $epoch . ' ' . $output_prefix);

    echo "0";
?>
