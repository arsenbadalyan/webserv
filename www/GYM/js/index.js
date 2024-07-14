function setCookie(name,value,days) {
	var expires = "";
	if (days) {
		var date = new Date();
		date.setTime(date.getTime() + (days*24*60*60*1000));
		expires = "; expires=" + date.toUTCString();
	}
	document.cookie = name + "=" + (value || "")  + expires + "; path=/";
}
function getCookie(name) {
	var nameEQ = name + "=";
	var ca = document.cookie.split(';');
	for(var i=0;i < ca.length;i++) {
		var c = ca[i];
		while (c.charAt(0)==' ') c = c.substring(1,c.length);
		if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
	}
	return null;
}
function eraseCookie(name) {   
	document.cookie = name +'=; Path=/; Expires=Thu, 01 Jan 1970 00:00:01 GMT;';
}

const USER_COOKIE_NAME = "gym-user-username";

window.onload = () => {
	const $loginForm = document.getElementById("login-form");
	const $logoutForm = document.getElementById("logout-form");
	const $setUser = document.getElementById("set-user");

	function syncUser() {
		const usernameCookie = getCookie(USER_COOKIE_NAME);

		if ($logoutForm) {
			$logoutForm.style.display = usernameCookie ? "block" : "none";
			$logoutForm.removeEventListener("submit", logoutFormHandler);
			$logoutForm.addEventListener("submit", logoutFormHandler);
		}
		if ($loginForm) {
			$loginForm.style.display = usernameCookie ? "none" : "block";
			$loginForm.removeEventListener("submit", loginFormHandler);
			$loginForm.addEventListener("submit", loginFormHandler);
		}

		if ($setUser) {
			$setUser.innerText = usernameCookie ? usernameCookie : "GUEST MODE";
		}

	}

	function logoutFormHandler(evt) {
		evt.stopPropagation();
		evt.preventDefault();

		eraseCookie(USER_COOKIE_NAME);
		syncUser();
	}

	function loginFormHandler(evt) {
		evt.stopPropagation();
		evt.preventDefault();

		const form = evt.target;
		const formData = new FormData(form);

		const username = formData.get("username");

		if (username) {
			setCookie(USER_COOKIE_NAME, username, 1);
			syncUser();
		}
	}

	syncUser();
};