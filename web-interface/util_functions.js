var global_uniq_id="";
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
