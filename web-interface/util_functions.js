$("#uploadForm").submit(function(event) {
    event.preventDefault();
    var fd = new FormData(document.getElementById('uploadForm'));
    $.ajax({
        url: "upload.php",
        type: "POST",
        data: fd,
        contentType: false,
        processData: false,
        success: function(str) {
            alert(str);
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
