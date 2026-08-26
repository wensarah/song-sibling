let canvas, ctx;
let points = [];
let minX, maxX, minY, maxY;
const clusterRadii = {
    twins: 0.01,
    siblings: 0.03,
    cousins: 0.08,
    wru: 0.3
};
let dotR = 6;
let selectDot = 8;
function getColor(i) {
    const hue = (i * 137.5) % 360;
    const lightness = 40 + (i % 3) * 15;
    `hsl(${hue}, 80%, ${lightness}%)`;
    return `hsl(${hue}, 70%, 50%)`;
}
function drawAllPoints() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    points.forEach(p => {
        const px = ((p.x - minX) / (maxX - minX)) * canvas.width;
        const py = ((p.y - minY) / (maxY - minY)) * canvas.height;
        ctx.beginPath();
        ctx.arc(px, py, 5, 0, 2 * Math.PI);
        ctx.fillStyle = 'rgb(114, 114, 114)';
        ctx.fill();
    });
}

function highlightPoint(point, color) {
    const px = ((point.x - minX) / (maxX - minX)) * canvas.width;
    const py = ((point.y - minY) / (maxY - minY)) * canvas.height;
    ctx.beginPath();
    ctx.arc(px, py, dotR, 0, 2 * Math.PI);
    ctx.fillStyle = color;
    ctx.fill();
}



fetch('http://localhost:8080/points')
    .then(res => res.json())
    .then(data => {
        points = data;

        canvas = document.getElementById('map');
        ctx = canvas.getContext('2d');

        const xs = points.map(p => p.x);
        const ys = points.map(p => p.y);
        minX = Math.min(...xs);
        maxX = Math.max(...xs);
        minY = Math.min(...ys);
        maxY = Math.max(...ys);

        drawAllPoints();
        const slider = document.getElementById("songSlider");
        canvas.addEventListener('click', (event) => {
            const px = event.offsetX;
            const py = event.offsetY;
            const qx = (px / canvas.width) * (maxX - minX) + minX;
            const qy = (py / canvas.height) * (maxY - minY) + minY;

            fetch(`http://localhost:8080/nearest?x=${qx}&y=${qy}`)
                .then(res => res.json())
                .then(nearest => {
                    drawAllPoints();
                    highlightPoint(nearest, 'pink');
                    //console.log('Nearest:', nearest.name);
                    songInput.value = nearest.name;
                    const songTitle = document.getElementById('currentSongName');
                    songTitle.textContent = nearest.name;
                    updateRadius(nearest, slider);
            });
        });
        
        const songInput = document.getElementById("songInput");
        const songList = document.getElementById("songList");
        points.forEach(p => {
            const option = document.createElement('option');
            option.value = p.name;
            songList.appendChild(option);
        });

        songInput.addEventListener('change', (event) => {
            let point = points.find(p=>p.name == songInput.value);
            const px = ((point.x - minX) / (maxX - minX)) * canvas.width;
            const py = ((point.y - minY) / (maxY - minY)) * canvas.height;
            drawAllPoints();
            ctx.beginPath();
            ctx.arc(px, py, dotR, 0, 2 * Math.PI);
            ctx.fillStyle = 'pink';
            ctx.fill();
            
            //draw an opaque circle
            const songTitle = document.getElementById('currentSongName');
            songTitle.textContent = point.name;

            updateRadius(point, slider);
        });

        slider.addEventListener('input', (event) => {
            let point = points.find(p => p.name === songInput.value);
            if (point) updateRadius(point, slider);
        })


        const clusterSelect = document.getElementById("clusterSelect");
        clusterSelect.addEventListener('change', (event) => {
            const tier = clusterSelect.value;
            if (!tier) return; // "cluster" placeholder option selected, do nothing
            const radius = clusterRadii[tier];

            fetch(`http://localhost:8080/cluster?radius=${radius}`)
                .then(res => res.json())
                .then(clusters => drawClusters(clusters));
        });
 });

function drawClusters(clusters) {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    clusters.forEach((cluster, i) => {
        const color = getColor(i);
        cluster.forEach(p => {
            const px = ((p.x - minX) / (maxX - minX)) * canvas.width;
            const py = ((p.y - minY) / (maxY - minY)) * canvas.height;
            ctx.beginPath();
            ctx.arc(px, py, dotR, 0, 2 * Math.PI);
            ctx.fillStyle = color;
            ctx.fill();
        });
    });
}


function updateRadius(point, slider, nearest) {
    let minRadius = 0.01;
    let maxRadius = 0.3;
    let sliderValue = slider.value;
    let radius = minRadius + (sliderValue / 100) * (maxRadius - minRadius);
    const pixelRadius = (radius / (maxX - minX)) * canvas.width;
    const px = ((point.x - minX) / (maxX - minX)) * canvas.width;
    const py = ((point.y - minY) / (maxY - minY)) * canvas.height;

    drawAllPoints();

    ctx.beginPath();
    ctx.arc(px, py, dotR, 0, 2 * Math.PI);
    ctx.fillStyle = 'pink';
    ctx.fill();

    ctx.beginPath();
    ctx.arc(px, py, pixelRadius, 0, 2 * Math.PI);
    ctx.fillStyle = 'rgba(255, 168, 240, 0.48)';
    ctx.fill();
    fetch(`http://localhost:8080/radius?x=${point.x}&y=${point.y}&radius=${radius}`)
    .then(res => res.json())
    .then(neighbors => {
        const neighborsList = document.getElementById('neighborsList');
        neighborsList.innerHTML = '';
        neighbors.forEach((item) => {
        // Create a new <li> element
        const bullet = document.createElement("li");
        
        // Set its text to the current array item
        bullet.textContent = item.name;
        
        // Add the <li> inside the <ul>
        neighborsList.appendChild(bullet);
        });
    });
}