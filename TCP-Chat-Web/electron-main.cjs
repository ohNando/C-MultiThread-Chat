const { app, BrowserWindow } = require('electron');
const path = require('path');

const VITE_DEV_SERVER_URL = 'http://localhost:5173'; 

function createWindow() {
    const mainWindow = new BrowserWindow({
        width: 1000, 
        height: 700,
        title: 'TCP Chat App - React/C Bridge',
        webPreferences: {
            
            nodeIntegration: false, 
            contextIsolation: true,
        },
    });

   mainWindow.loadURL(VITE_DEV_SERVER_URL);
   mainWindow.webContents.openDevTools();
}

app.whenReady().then(() => {
    createWindow();

    app.on('activate', () => {
        if (BrowserWindow.getAllWindows().length === 0) {
            createWindow();
        }
    });
});

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit();
    }
});