<?php
    /* Returns the status of the dump files.
     * Returns 0 if the driver has started dumping output.
     * Returns 1 if the driver has not yet started dumping output.
     * Returns 2 if the driver has finished running.
     */ 
    
    $history = intval($_POST['history']);
    $uid = $_POST['uid'];

    $latest_history = get_latest_history('./Results/' . $uid . '_run/' . $uid . '_input_*.csv'); 
    if($latest_history == -1 || $latest_history < $history) {
        echo "Latest history error";
        return;
    }

    if($latest_history > $history) {
        echo "2";
        return;
    }

    if($latest_history == $history) {
        if(file_exists('./Results/' . $uid . '_run/' . $uid . '_weights_dump.csv')) {
            echo "0";
            return;
        } else {
            echo "1";
            return;
        }
    }
?>
