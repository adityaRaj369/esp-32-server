const express = require('express');
const fileUpload = require('express-fileupload');
const fs = require('fs');
const path = require('path');

const app = express();
const PORT = process.env.PORT || 5000; // Render assigns a port via process.env.PORT
const UPLOAD_DIR = 'uploads';

app.use(fileUpload());
app.use(express.static('public'));

if (!fs.existsSync(UPLOAD_DIR)) {
    fs.mkdirSync(UPLOAD_DIR);
}

app.get('/', (req, res) => {
    res.send(`
        <!DOCTYPE html>
        <html>
        <body>
            <h2>Upload File to ESP32 Server</h2>
            <form action="/upload" method="post" enctype="multipart/form-data">
                <input type="file" name="file" required><br><br>
                <input type="submit" value="Upload">
            </form>
        </body>
        </html>
    `);
});

app.post('/upload', (req, res) => {
    if (!req.files || !req.files.file) {
        return res.status(400).send('No file uploaded.');
    }
    const file = req.files.file;
    const filePath = path.join(UPLOAD_DIR, file.name);

    file.mv(filePath, (err) => {
        if (err) {
            console.error('Error saving file:', err);
            return res.status(500).send('Error saving file.');
        }
        console.log(`Uploaded: ${file.name}`);
        res.send(`File ${file.name} uploaded successfully!`);
    });
});

app.get('/files', (req, res) => {
    fs.readdir(UPLOAD_DIR, (err, files) => {
        if (err) {
            console.error('Error reading files:', err);
            return res.status(500).send('Error reading files.');
        }
        res.json(files);
    });
});

app.get('/download/:filename', (req, res) => {
    const filePath = path.join(UPLOAD_DIR, req.params.filename);
    if (fs.existsSync(filePath)) {
        res.download(filePath);
    } else {
        res.status(404).send('File not found.');
    }
});

app.listen(PORT, '0.0.0.0', () => {
    console.log(`Server running at http://0.0.0.0:${PORT}`);
});