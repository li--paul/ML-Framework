<?php
    /* Defines php functions widely used */

    /*
     * Search for files of the given pattern,
     * extract the history and return the latest
     */
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

    /*
     * Get the dimensions of the given csv file
     */
    function get_dimension($file) {
        $f = fopen($file, 'r');
        if($f == false) {
            echo "Cannot open file";
            return -1;
        }
        $line = fgets($f);
        fclose($f);

        $dim = 0;

        $line_parts = explode(',', $line);
        foreach ($line_parts as $lp) {
            if($lp != "") {
                $dim++;
            }
        }

        return $dim;
    }

?>
