<?php
    /* 
     * Echo's the file contents on success;
     * Echo's -1 on failure. (It is most likely that
     * '-1' could not be a file content!, so treat it 
     * as failure)
     */
    $fpath = $_POST['fpath'];

    $contents = file_get_contents($fpath);

    if($contents) {
        echo $contents;
    } else {
        echo -1;
    }
?>
