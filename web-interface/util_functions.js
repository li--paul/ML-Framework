var global_uniq_id="";

function run_regression(uid) {
    // Start execution
    var parameter_map = {
                        "uid"             : uid,
                        "learning_rate"   : "0",
                        "epoch"           : "-1",
                        "prefix"          : uid,
                        "refine"          : "no" };
    $.ajax({
        url: "process_csv.php",
        type: "POST",
        data: parameter_map,
        success: function(str) {
            alert(str); 
        }
    });
    

    // Update plot
}

/*
 * Return true if file is a valid csv,
 * Return false if file is invalid csv.
 */
function validate_csv(filename) {
    var file_parts = filename.split(".");
    var extn = file_parts[file_parts.length - 1];
    if(extn.localeCompare("csv") == 0) {
        return true;
    } else {
        return false;
    }
}

$("#uploadForm").submit(function(event) {
    event.preventDefault();

    var input_filename = document.getElementById('filename_input').value;
    var weights_filename = document.getElementById('filename_weights').value;
    var learning_rate = parseFloat(document.getElementById('user_learning_rate').value);
    var epoch = parseFloat(document.getElementById('user_epoch').value);

    // Validation
    if(!validate_csv(input_filename)) {
        alert('Invalid input filename')
        return;
    } 

    if(!validate_csv(weights_filename)) {
        /* Random weights */
        weights_filename = 0;
        /* Reflect the same in fileinput weights
         * to avoid unnecessary upload
         */
        document.getElementById('fileinput_weights').value = "";
    }

    if(isNaN(learning_rate) || learning_rate < 0) {
        /* Use default learning rate */
        learning_rate = 0;
    }

    if(isNaN(epoch) || epoch <= 0) {
        /* Using default value */
        epoch = -1;
    }

    alert("Process arguments : " + "\n" + " Input file          - " + input_filename  + "\n" + " Input weights file  -" + weights_filename + "\n" + " Learning rate       -" + learning_rate.toString() + "\n" + " Epch                -" + epoch.toString() + "\n");

    // Get unique id
    $.ajax({
        url: "get_unique_id.php",
        type: "GET",
        success: function(uniq_id) {
            // Try to upload file
            global_uniq_id = uniq_id;

            var fd = new FormData(document.getElementById('uploadForm'));
            fd.append("uniq_id", uniq_id);
            fd.append("validated_weights_filename", weights_filename);
            $.ajax({
                url: "upload.php",
                type: "POST",
                data: fd,
                contentType: false,
                processData: false,
                success: function(str) {
                    if(str.localeCompare("000") == 0) {
                        alert("Upload success... Processing");
    
                        // Run regression
                        //run_regression(uniq_id);

                    } else {
                        alert(str + "... Aborting" );
                    }
                }
            });
        }
    });
});

function selectFileInput() {
    var fi = document.getElementById('fileinput_input');
    fi.click();
} 

function selectFileWeights() {
    var fi = document.getElementById('fileinput_weights');
    fi.click();
} 

function reflectChangeInput() {
    var fileinput_input_element = document.getElementById('fileinput_input');
    if(fileinput_input_element.value.localeCompare("") == 0) {
        document.getElementById('filename_input').value = "Click to select input file"; 
    } else {
        document.getElementById('filename_input').value = fileinput_input_element.value; 
    }
}

function reflectChangeWeights() {
    var fileinput_weights_element = document.getElementById('fileinput_weights');
    if(fileinput_weights_element.value.localeCompare("") == 0) {
        document.getElementById('filename_weights').value = "Click to select weights file";
    } else {
        document.getElementById('filename_weights').value = fileinput_weights_element.value;
    }
}
