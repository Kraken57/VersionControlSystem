// script.js
let branchCount = 1;

function createBranch() {
  const branchContainer = document.getElementById("branch-container");
  const newBranch = document.createElement("div");
  newBranch.className = "branch";
  newBranch.innerHTML = `<div class="label">Branch ${branchCount}</div>`;
  branchContainer.appendChild(newBranch);

  // Create an arrow
  const arrow = document.createElement("div");
  arrow.className = "arrow";
  branchContainer.appendChild(arrow);

  branchCount++;
}

function mergeBranch() {
  const branchContainer = document.getElementById("branch-container");

  if (branchCount > 1) {
    // Merge the last branch with the main branch
    branchContainer.removeChild(branchContainer.lastChild); // Remove the arrow
    branchContainer.removeChild(branchContainer.lastChild); // Remove the branch
    branchCount--;
  }
}
