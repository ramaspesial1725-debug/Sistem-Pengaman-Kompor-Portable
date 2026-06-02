// ===== SP3S - Smart Portable Stove Safety System =====
// script.js — Full logic for login, dashboard, simulation

(function () {
    'use strict';

    // ===== DOM ELEMENTS =====
    const loginPage = document.getElementById('loginPage');
    const dashboardPage = document.getElementById('dashboardPage');
    const ipInput = document.getElementById('ipInput');
    const connectBtn = document.getElementById('connectBtn');
    const errorMessage = document.getElementById('errorMessage');
    const headerIP = document.getElementById('headerIP');
    const headerTime = document.getElementById('headerTime');
    const cardGas = document.getElementById('cardGas');
    const gasStatusText = document.getElementById('gasStatusText');
    const gasIconSafe = document.getElementById('gasIconSafe');
    const gasIconWarning = document.getElementById('gasIconWarning');
    const gasIconDanger = document.getElementById('gasIconDanger');
    const tempValue = document.getElementById('tempValue');
    const valveStatus = document.getElementById('valveStatus');
    const alarmStatus = document.getElementById('alarmStatus');
    const wifiStatus = document.getElementById('wifiStatus');
    const historyBody = document.getElementById('historyBody');
    const modalHistoryBody = document.getElementById('modalHistoryBody');
    const btnViewAll = document.getElementById('btnViewAll');
    const historyModal = document.getElementById('historyModal');
    const modalClose = document.getElementById('modalClose');
    const btnDisconnect = document.getElementById('btnDisconnect');

    // ===== STATE =====
    let historyData = [];
    let lastStatus = null;
    let simulationInterval = null;

    // ===== INIT =====
    function init() {
        // Check if already connected
        const savedIP = localStorage.getItem('sp3s_ip');
        if (savedIP) {
            showDashboard(savedIP);
        }

        // Event listeners
        connectBtn.addEventListener('click', connectToDevice);
        ipInput.addEventListener('keypress', function (e) {
            if (e.key === 'Enter') connectToDevice();
        });
        ipInput.addEventListener('input', function () {
            errorMessage.textContent = '';
            ipInput.classList.remove('input-error');
        });

        btnViewAll.addEventListener('click', openModal);
        modalClose.addEventListener('click', closeModal);
        historyModal.addEventListener('click', function (e) {
            if (e.target === historyModal) closeModal();
        });

        btnDisconnect.addEventListener('click', disconnectDevice);

        // Update clock
        updateClock();
        setInterval(updateClock, 1000);
    }

    // ===== IP VALIDATION =====
    function validateIP(ip) {
        const pattern = /^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/;
        const match = ip.match(pattern);
        if (!match) return false;
        return match.slice(1).every(function (octet) {
            const num = parseInt(octet, 10);
            return num >= 0 && num <= 255;
        });
    }

    // ===== CONNECT =====
    window.connectToDevice = function () {
        const ip = ipInput.value.trim();

        if (!ip) {
            showError('Silakan masukkan alamat IP');
            return;
        }

        if (!validateIP(ip)) {
            showError('Format IP tidak valid! Contoh: 192.168.1.1');
            return;
        }

        // Save and redirect
        localStorage.setItem('sp3s_ip', ip);
        showDashboard(ip);
    };

    // ===== DISCONNECT =====
    function disconnectDevice() {
        if (!confirm('Putuskan koneksi dan kembali ke halaman login?')) return;

        // Stop simulation
        if (simulationInterval) {
            clearInterval(simulationInterval);
            simulationInterval = null;
        }

        // Clear saved IP
        localStorage.removeItem('sp3s_ip');

        // Reset state
        historyData = [];
        lastStatus = null;
        historyBody.innerHTML = '';

        // Reset gas card
        cardGas.classList.remove('status-aman', 'status-warning', 'status-bahaya');
        gasStatusText.classList.remove('status-aman', 'status-warning', 'status-bahaya');
        gasStatusText.textContent = 'AMAN';
        gasIconSafe.style.display = 'block';
        gasIconWarning.style.display = 'none';
        gasIconDanger.style.display = 'none';

        // Reset temp
        tempValue.textContent = '28.5°';

        // Reset system
        valveStatus.textContent = 'TERBUKA';
        valveStatus.classList.remove('valve-closed');
        alarmStatus.textContent = 'NONAKTIF';
        alarmStatus.classList.remove('alarm-active');
        alarmStatus.classList.add('alarm-off');

        // Clear input
        ipInput.value = '';
        errorMessage.textContent = '';
        ipInput.classList.remove('input-error');

        // Switch pages
        dashboardPage.style.display = 'none';
        loginPage.style.display = 'flex';
    }

    function showError(msg) {
        errorMessage.textContent = msg;
        ipInput.classList.add('input-error');
    }

    // ===== SHOW DASHBOARD =====
    function showDashboard(ip) {
        loginPage.style.display = 'none';
        dashboardPage.style.display = 'flex';
        headerIP.textContent = ip;

        // Add initial class
        cardGas.classList.add('status-aman');

        // Start simulation
        generateData();
        simulationInterval = setInterval(generateData, 5000);
    }

    // ===== CLOCK =====
    function updateClock() {
        const now = new Date();
        const hours = String(now.getHours()).padStart(2, '0');
        const minutes = String(now.getMinutes()).padStart(2, '0');
        headerTime.textContent = hours + '.' + minutes;
    }

    // ===== DATA SIMULATION =====
    function generateData() {
        // Random temperature between 25 and 80
        const temp = (Math.random() * 55 + 25).toFixed(1);
        const tempNum = parseFloat(temp);

        // Determine status
        let status;
        if (tempNum < 40) {
            status = 'AMAN';
        } else if (tempNum <= 60) {
            status = 'WARNING';
        } else {
            status = 'BAHAYA';
        }

        // Update UI
        updateTemperature(temp);
        updateGasStatus(status);
        updateSystemStatus(status);

        // Add to history only on status change or first run
        if (status !== lastStatus || lastStatus === null) {
            addHistoryEntry(status, temp);
            lastStatus = status;
        }
    }

    // ===== UPDATE TEMPERATURE =====
    function updateTemperature(temp) {
        tempValue.textContent = temp + '°';
        // Animate the value change
        tempValue.style.transform = 'scale(1.1)';
        setTimeout(function () {
            tempValue.style.transform = 'scale(1)';
        }, 300);
    }

    // ===== UPDATE GAS STATUS =====
    function updateGasStatus(status) {
        // Reset
        cardGas.classList.remove('status-aman', 'status-warning', 'status-bahaya');
        gasStatusText.classList.remove('status-aman', 'status-warning', 'status-bahaya');
        gasIconSafe.style.display = 'none';
        gasIconWarning.style.display = 'none';
        gasIconDanger.style.display = 'none';

        switch (status) {
            case 'AMAN':
                cardGas.classList.add('status-aman');
                gasStatusText.classList.add('status-aman');
                gasStatusText.textContent = 'AMAN';
                gasIconSafe.style.display = 'block';
                break;
            case 'WARNING':
                cardGas.classList.add('status-warning');
                gasStatusText.classList.add('status-warning');
                gasStatusText.textContent = 'WARNING';
                gasIconWarning.style.display = 'block';
                break;
            case 'BAHAYA':
                cardGas.classList.add('status-bahaya');
                gasStatusText.classList.add('status-bahaya');
                gasStatusText.textContent = 'BAHAYA';
                gasIconDanger.style.display = 'block';
                break;
        }
    }

    // ===== UPDATE SYSTEM STATUS =====
    function updateSystemStatus(status) {
        // Valve
        valveStatus.classList.remove('valve-closed');
        if (status === 'BAHAYA') {
            valveStatus.textContent = 'TERTUTUP';
            valveStatus.classList.add('valve-closed');
        } else {
            valveStatus.textContent = 'TERBUKA';
        }

        // Alarm
        alarmStatus.classList.remove('alarm-off', 'alarm-active');
        if (status === 'BAHAYA') {
            alarmStatus.textContent = 'AKTIF';
            alarmStatus.classList.add('alarm-active');
        } else {
            alarmStatus.textContent = 'NONAKTIF';
            alarmStatus.classList.add('alarm-off');
        }

        // WiFi always connected in simulation
        wifiStatus.textContent = 'TERHUBUNG';
        wifiStatus.classList.add('wifi-connected');
    }

    // ===== HISTORY =====
    function addHistoryEntry(status, temp) {
        const now = new Date();
        const day = String(now.getDate()).padStart(2, '0');
        const month = String(now.getMonth() + 1).padStart(2, '0');
        const hours = String(now.getHours()).padStart(2, '0');
        const minutes = String(now.getMinutes()).padStart(2, '0');

        const entry = {
            date: day + '/' + month,
            time: hours + '.' + minutes,
            status: status,
            temp: temp + '°C'
        };

        historyData.unshift(entry);

        // Update main table (show last 5)
        renderHistoryTable();
    }

    function renderHistoryTable() {
        const displayData = historyData.slice(0, 5);
        historyBody.innerHTML = '';

        displayData.forEach(function (entry, index) {
            const row = document.createElement('tr');
            row.style.animationDelay = (index * 0.05) + 's';

            let badgeClass = '';
            switch (entry.status) {
                case 'AMAN': badgeClass = 'badge-aman'; break;
                case 'WARNING': badgeClass = 'badge-warning'; break;
                case 'BAHAYA': badgeClass = 'badge-bahaya'; break;
            }

            row.innerHTML =
                '<td>' + entry.date + '</td>' +
                '<td>' + entry.time + '</td>' +
                '<td><span class="status-badge ' + badgeClass + '">' + entry.status + '</span></td>' +
                '<td>' + entry.temp + '</td>';

            historyBody.appendChild(row);
        });
    }

    // ===== MODAL =====
    function openModal() {
        modalHistoryBody.innerHTML = '';

        historyData.forEach(function (entry, index) {
            const row = document.createElement('tr');
            row.style.animationDelay = (index * 0.03) + 's';

            let badgeClass = '';
            switch (entry.status) {
                case 'AMAN': badgeClass = 'badge-aman'; break;
                case 'WARNING': badgeClass = 'badge-warning'; break;
                case 'BAHAYA': badgeClass = 'badge-bahaya'; break;
            }

            row.innerHTML =
                '<td>' + entry.date + '</td>' +
                '<td>' + entry.time + '</td>' +
                '<td><span class="status-badge ' + badgeClass + '">' + entry.status + '</span></td>' +
                '<td>' + entry.temp + '</td>';

            modalHistoryBody.appendChild(row);
        });

        historyModal.style.display = 'flex';
        document.body.style.overflow = 'hidden';
    }

    function closeModal() {
        historyModal.style.display = 'none';
        document.body.style.overflow = '';
    }

    // ===== ESC KEY TO CLOSE MODAL =====
    document.addEventListener('keydown', function (e) {
        if (e.key === 'Escape' && historyModal.style.display === 'flex') {
            closeModal();
        }
    });

    // ===== START =====
    document.addEventListener('DOMContentLoaded', init);

})();
