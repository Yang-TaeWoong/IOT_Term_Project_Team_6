package kr.or.connect.mountFire.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;

@Controller
public class DBcontroller {
	@GetMapping(path="/form")
	public String plusform() {
		return "form";
	}
}
