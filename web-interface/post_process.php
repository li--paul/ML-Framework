<?php

    include 'php_utils.php';

    $uid = $argv[1];

    /* Get previous history */
    $history = get_latest_history('*_input_*.csv');
    if($history == -1) {
        echo "No history";
        return;
    }

    $latest_history = $history + 1;

    /* Rename input and weights dump with new history */
    if(!rename($uid . '_input_dump.csv', $uid . '_input_' . strval($latest_history) . '.csv')) {
        echo "Rename fail!";
        return;
    }
    if(!rename($uid . '_weights_dump.csv', $uid . '_weights_' . strval($latest_history) . '.csv')) {
        echo "Rename fail!";
        return;
    }
?>
