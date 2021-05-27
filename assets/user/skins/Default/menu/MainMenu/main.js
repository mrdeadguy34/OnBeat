//Required JS functions

//JS calls into C api
function applySettings() {
    let config = {};
    for (let setting of document.getElementsByClassName("configValue")) {
        if (setting.value.length != 0) config[setting.id] = setting.value;
    }
    PushSettings(config);
}   

function cancelSettings() {
    RevertSettings();
}

//C callbacks to JS
function setSettings(stringSettings) {
    var settings = JSON.parse(stringSettings);
    for (let key in settings) {
        let setting = document.getElementById(key);
        if (!setting) {
            continue;
        }
        setting.value = settings[key];    
                
    }
}

//Non required JS functions

//JS functions
function display(element, hide, style) {
    for (let ele of hide) {
        ele.style.display = "none";
    }

    element.style.display = style;
}


//Event listeners
GameSettings.addEventListener("click", () => display(Settings,
 document.body.children, "block"));

DisplayHeader.addEventListener("click", () => display(
    DisplaySettings, document.getElementsByClassName("settingsOptions"), "flex"));

InputHeader.addEventListener("click", () => display(
    InputSettings, document.getElementsByClassName("settingsOptions"), "flex"));

AudioHeader.addEventListener("click", () => display(
    AudioSettings, document.getElementsByClassName("settingsOptions"), "flex"));


SettingsQuit.addEventListener("click", () => display(MainMenu,
 document.body.children, "block"));

VolumeSelect.onchange = function() {
    Volume.value = this.value;
}

Volume.onchange = function() {
    if (isNaN(this.value) || Number(this.value) > this.max || Number(this.value) < this.min) {
        this.value = VolumeSelect.value;
        return;
    }
    VolumeSelect.value = this.value;
}

//Initial listener calls
DisplayHeader.click();