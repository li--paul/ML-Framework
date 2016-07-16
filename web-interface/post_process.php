<?php
    $uid = $argv[1];

    $farray = glob('*_input_*.csv');
    
    if(count($farray) == 0) {
        echo "No input csv files";
        return;
    }

    /* Get previous history */
    $history = -1;
    foreach ($farray as $f) {
        $all_uscore = str_replace('.', '_', $f);
        $fparts = explode('_', $all_uscore);
        $this_history = intval($fparts[count($fparts) - 2]);
        if($this_history > $history) {
            $history = $this_history;
        }
    }

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
