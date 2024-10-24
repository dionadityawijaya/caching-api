async function fetchDataFromAPI() {
    try {
        const response = await fetch('http://localhost:8000/api/user/1'); // Fetching data for user/1
        const data = await response.json();

        // Simpan data yang diambil ke localStorage sebagai cache sisi klien
        localStorage.setItem('userData', JSON.stringify(data));

        // Update tampilan UI
        displayCachedData();
    } catch (error) {
        console.error("Error fetching data from API:", error);
    }
}

function displayCachedData() {
    const cachedData = JSON.parse(localStorage.getItem('userData')) || [];
    const dataList = document.getElementById('dataList');
    dataList.innerHTML = '';

    if (cachedData.length === 0) {
        dataList.innerHTML = '<li>No cached data available</li>';
    } else {
        const listItem = document.createElement('li');
        listItem.textContent = `User: ${cachedData.user}`;
        dataList.appendChild(listItem);
    }
}

document.getElementById('fetchDataBtn').addEventListener('click', fetchDataFromAPI);
window.onload = displayCachedData;
