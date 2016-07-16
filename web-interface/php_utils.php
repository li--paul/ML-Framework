<?php
    /* Defines php functions widely used */

    /* Should be called from */
    function get_latest_history($pattern) {
        $farray = glob($pattern);

        if(count($farray) == 0) {
            echo "Pattern not present ! ";
            return -1;
        }

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
            echo "History at -1";
            return -1;
        }

        return $history;
    }
?>
