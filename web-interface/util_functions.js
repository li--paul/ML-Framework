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

$("#uploadForm").submit(function(event) {
    event.preventDefault();

    // Get unique id
    $.ajax({
        url: "get_unique_id.php",
        type: "GET",
        success: function(uniq_id) {
            // Try to upload file
            global_uniq_id = uniq_id;

            var fd = new FormData(document.getElementById('uploadForm'));
            fd.append("uniq_id", uniq_id);
            $.ajax({
                url: "upload.php",
                type: "POST",
                data: fd,
                contentType: false,
                processData: false,
                success: function(str) {
                    if(str.localeCompare("Upload success.") == 0) {
                        alert("Upload success... Processing");
    
                        // Run regression
                        run_regression(uniq_id);

                    } else {
                        alert(str + "... Aborting");
                    }
                }
            });
        }
    });
});

function selectFile() {
    var fi = document.getElementById('fileinput');
    fi.click();
} 

function reflectChange() {
    document.getElementById('filename').value = document.getElementById('fileinput').value;
}
