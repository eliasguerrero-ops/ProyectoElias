let count = 0;

const counterDisplay = document.getElementById('counter');
const btnIncrement = document.getElementById('btn-increment');
const btnReset = document.getElementById('btn-reset');

btnIncrement.addEventListener('click', () => {
  count++;
  counterDisplay.textContent = count;
});

btnReset.addEventListener('click', () => {
  count = 0;
  counterDisplay.textContent = count;
});