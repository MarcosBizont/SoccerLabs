<?php
/**
 * Copyright 2014 Colectivo Labs
 * Marcos Castillo
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

if (!function_exists('curl_init')) {
  throw new Exception('Tip Gin api needs the CURL PHP extension.');
}
if (!function_exists('json_decode')) {
  throw new Exception('Tip Gin api needs the JSON PHP extension.');
}

class TipGinApi 
{
	const TGAPI_ENDPOINT = 'http://www.tipgin.net/datav2/accounts/';
	const TGAPI_SPORTPREFIX = '/soccer/';
	const TGAPI_PATH = '/var/www/html/sites/all/libraries/tipgin-api/';
	
	public function __construct($apikey) {
		
	$this->TGAPI_LIVESCORE_COUNTRIES = array('livescore','d-1');
	$this->TGAPI_FIXTURES_COUNTRIES = array('africa', 'albania', 'algeria', 'austria', 'algeria', 'angola', 'argentina', 'armenia', 'asia', 'australia', 'azerbaijan', 'belarus', 'belgium', 'bolivia', 'bosnia', 'brazil', 'bulgaria', 'cameroon', 'canada', 'chile', 'china', 'colombia', 'concacaf', 'congo', 'croatia', 'cyprus', 'czech', 'costarica', 'denmark', 'equador', 'egypt', 'elsalvador', 'england', 'estonia', 'europe', 'finland', 'france', 'georgia', 'germany', 'ghana', 'greece', 'guatemala', 'holland', 'honduras', 'hungary', 'iceland', 'india', 'indonesia', 'international', 'iran', 'ireland', 'israel', 'italy', 'japan', 'jordan', 'kazakhstan', 'kenya', 'korea', 'kuwait', 'latvia', 'lithuania', 'macedonia', 'malaysia', 'malta', 'mexico', 'moldova', 'montenegro', 'morocco', 'newzealand', 'nigeria', 'norway', 'oceania', 'paraguay', 'peru', 'poland', 'portugal', 'qatar', 'romania', 'russia', 'saudiarabia', 'scotland', 'serbia', 'singapore', 'slovakia', 'slovenia', 'southafrica', 'southamerica', 'spain', 'sweden', 'switzerland', 'thailand', 'tunisia', 'turkey', 'uae', 'usa', 'ukraine', 'uruguay', 'uzbekistan', 'venezuela', 'vietnam', 'wales', 'worldcup' );
	$this->TGAPI_RECENT_RESULTS_COUNTRIES = array('africa', 'albania', 'algeria', 'austria', 'algeria', 'angola', 'argentina', 'armenia', 'asia', 'australia', 'azerbaijan', 'belarus', 'belgium', 'bolivia', 'bosnia', 'brazil', 'bulgaria', 'cameroon', 'canada', 'chile', 'china', 'colombia', 'concacaf', 'congo', 'croatia', 'cyprus', 'czech', 'costarica', 'denmark', 'equador', 'egypt', 'elsalvador', 'england', 'estonia', 'europe', 'finland', 'france', 'georgia', 'germany', 'ghana', 'greece', 'guatemala', 'holland', 'honduras', 'hungary', 'iceland', 'india', 'indonesia', 'international', 'iran', 'ireland', 'israel', 'italy', 'japan', 'jordan', 'kazakhstan', 'kenya', 'korea', 'kuwait', 'latvia', 'lithuania', 'macedonia', 'malaysia', 'malta', 'mexico', 'moldova', 'montenegro', 'morocco', 'newzealand', 'nigeria', 'norway', 'oceania', 'paraguay', 'peru', 'poland', 'portugal', 'qatar', 'romania', 'russia', 'saudiarabia', 'scotland', 'serbia', 'singapore', 'slovakia', 'slovenia', 'southafrica', 'southamerica', 'spain', 'sweden', 'switzerland', 'thailand', 'tunisia', 'turkey', 'uae', 'usa', 'ukraine', 'uruguay', 'uzbekistan', 'venezuela', 'vietnam', 'wales', 'worldcup');
	$this->TGAPI_SEASON_RESULTS_COUNTRIES = array('albania', 'algeria', 'andorra', 'angola', 'argentina', 'armenia', 'aruba', 'australia', 'austria', 'azerbaijan', 'bahrain', 'bangladesh', 'barbados', 'belarus', 'belgium', 'belize', 'bermuda', 'bhutan', 'bolivia', 'bosnia', 'botswana', 'brazil', 'brunei', 'bulgaria', 'cambodia', 'cameroon', 'canada', 'chile', 'china', 'chinesetaipei', 'colombia', 'costarica', 'croatia', 'cyprus', 'czech', 'denmark', 'ecuador', 'egypt', 'elsalvador', 'england', 'estonia', 'europe', 'faroeislands', 'fiji', 'finland', 'france', 'friendly', 'gabon', 'georgia', 'germany', 'ghana', 'greece', 'grenada', 'guadeloupe', 'guatemala', 'haiti', 'holland', 'honduras', 'hongkong', 'hungary', 'iceland', 'india', 'indonesia', 'international', 'iran', 'iraq', 'ireland', 'israel', 'italy', 'ivorycoast', 'jamaica', 'japan', 'jordan', 'kazakhstan', 'korea', 'kuwait', 'latvia', 'lebanon', 'libya', 'lithuania', 'luxembourg', 'macedonia', 'malaysia', 'malta', 'mexico', 'moldova', 'montenegro', 'morocco', 'namibia', 'nepal', 'newzealand', 'nicaragua', 'nigeria', 'northernireland', 'norway', 'oman', 'pakistan', 'panama', 'paraguay', 'peru', 'poland', 'portugal', 'qatar', 'romania', 'russia', 'sanmarino', 'saudiarabia', 'scotland', 'senegal', 'serbia', 'singapore', 'slovakia', 'slovenia', 'southafrica', 'spain', 'sudan', 'surinam', 'sweden', 'switzerland', 'syria', 'thailand', 'trinidadandtobago', 'tunisia', 'turkey', 'uae', 'ukraine', 'uruguay', 'usa', 'uzbekistan', 'venezuela', 'vietnam', 'wales', 'worldcup', 'yemen');
	$this->TGAPI_STANDINGS_COUNTRIES = array('africa', 'albania', 'algeria', 'andorra', 'angola', 'argentina', 'armenia', 'aruba', 'asia',  'australia', 'austria', 'azerbaijan', 'bahrain', 'bangladesh', 'barbados', 'belarus', 'belgium', 'belize', 'bermuda', 'bhutan', 'bolivia', 'bosnia', 'botswana', 'brazil', 'brunei', 'bulgaria', 'cambodia', 'cameroon', 'canada', 'chile', 'china', 'chinesetaipei', 'colombia', 'costarica', 'ivorycoast', 'croatia', 'cyprus', 'czech', 'denmark', 'dominicanrepublic', 'ecuador', 'egypt', 'elsalvador', 'england', 'estonia', 'europe', 'faroeislands', 'finland', 'france', 'gabon', 'georgia', 'germany', 'ghana', 'greece', 'grenada', 'guadeloupe', 'guatemala', 'haiti', 'honduras', 'hongkong', 'hungary', 'iceland', 'india', 'indonesia', 'iran', 'iraq', 'ireland', 'israel', 'italy', 'jamaica', 'japan', 'jordan', 'kazakhstan', 'korea', 'kuwait', 'latvia', 'lebanon', 'libya', 'lithuania', 'luxembourg', 'macedonia', 'malaysia', 'malta', 'mexico', 'moldova', 'montenegro', 'morocco', 'namibia', 'nepal', 'netherlands', 'newzealand', 'nigeria', 'northernireland', 'norway', 'oman', 'pakistan', 'panama', 'paraguay', 'peru', 'poland', 'portugal', 'qatar', 'romania', 'russia', 'saudiarabia', 'scotland', 'senegal', 'serbia', 'singapore', 'slovakia', 'slovenia', 'southafrica', 'southamerica',  'spain', 'sudan', 'surinam', 'sweden', 'switzerland', 'syria', 'thailand', 'trinidadandtobago', 'tunisia', 'turkey', 'ukraine', 'uae', 'uruguay', 'usa', 'uzbekistan', 'venezuela', 'vietnam', 'wales', 'worldcup', 'yemen');
	$this->TGAPI_SCORERS_COUNTRIES = array('albania', 'algeria', 'argentina', 'armenia', 'australia', 'austria', 'azerbaijan', 'belarus', 'belgium', 'bosnia', 'brazil', 'bulgaria', 'chile', 'china', 'colombia', 'costarica', 'croatia', 'cyprus', 'czech', 'denmark', 'ecuador', 'egypt', 'england', 'estonia', 'finland', 'france', 'georgia', 'germany', 'greece', 'holland', 'hungary', 'iceland', 'ireland', 'israel', 'italy', 'japan', 'kazakhstan', 'korea', 'kuwait', 'latvia', 'lithuania', 'macedonia', 'mexico', 'moldova', 'morocco', 'norway', 'paraguay', 'peru', 'poland', 'qatar', 'romania', 'russia', 'saudiarabia', 'scotland', 'serbia', 'slovakia', 'slovenia', 'spain', 'sweden', 'switzerland', 'turkey', 'uae', 'ukraine', 'uruguay', 'usa'); 
	$this->TGAPI_ODDS_COUNTRIES = array('africa', 'albania', 'algeria', 'argentina', 'austria', 'algeria', 'angola', 'armenia', 'asia', 'australia', 'azerbaijan', 'belarus', 'belgium', 'bolivia', 'bosnia', 'brazil', 'bulgaria', 'cameroon', 'canada', 'chile', 'china', 'colombia', 'concacaf', 'congo', 'croatia', 'cyprus', 'czech', 'costarica', 'denmark', 'equador', 'egypt', 'elsalvador', 'england', 'estonia', 'europe', 'finland', 'france', 'georgia', 'germany', 'ghana', 'greece', 'guatemala', 'holland', 'honduras', 'hungary', 'iceland', 'india', 'indonesia', 'international', 'iran', 'ireland', 'israel', 'italy', 'japan', 'jordan', 'kazakhstan', 'kenya', 'korea', 'kuwait', 'latvia', 'lithuania', 'macedonia', 'malaysia', 'malta', 'mexico', 'moldova', 'montenegro', 'morocco', 'newzealand', 'nigeria', 'norway', 'oceania', 'paraguay', 'peru', 'poland', 'portugal', 'qatar', 'romania', 'russia', 'saudiarabia', 'scotland', 'serbia', 'singapore', 'slovakia', 'slovenia', 'southafrica', 'southamerica', 'spain', 'sweden', 'switzerland', 'thailand', 'tunisia', 'turkey', 'uae', 'usa', 'ukraine', 'uruguay', 'uzbekistan', 'venezuela', 'vietnam', 'wales', 'worldcup'); 
	
		$this->apikey = $apikey;
	}
	
	// To use as testing, every file must bring accord as needed with individual calls
	public function getAllFiles()
	{
		$this->getLivescoreFile();
		$this->getFixturesFile();
		$this->getRecentResultsFile();
		$this->getSeasonsResultsFile();
		$this->getStandingsFile();
		$this->getScorersFile();
		$this->getOddsFile();
	}
	// livescore
	public function getLivescoreFile()
	{
		$countries = $this->TGAPI_LIVESCORE_COUNTRIES;
		foreach($countries as $country)
		{
			$this->_getFile('livescore',$country);
		}		
	}
	// fixtures
	public function getFixturesFile()
	{
		$countries = $this->TGAPI_FIXTURES_COUNTRIES;
		foreach($countries as $country)
		{
			$this->_getFile('fixtures',$country);
		}		
	}
	// recent_results
	public function getRecentResultsFile()
	{
		$countries = $this->TGAPI_RECENT_RESULTS_COUNTRIES;
		foreach($countries as $country)
		{
			$this->_getFile('recent_results',$country);
		}		
	}
	// results
	public function getSeasonsResultsFile()
	{
		$countries = $this->TGAPI_SEASON_RESULTS_COUNTRIES;
		foreach($countries as $country)
		{
			$this->_getFile('results',$country);
		}		
	}
	// standings
	public function getStandingsFile()
	{
		$countries = $this->TGAPI_STANDINGS_COUNTRIES;
		foreach($countries as $country)
		{
			$this->_getFile('standings',$country);
		}		
	}
	// scorers
	public function getScorersFile()
	{
		$countries = $this->TGAPI_SCORERS_COUNTRIES;
		foreach($countries as $country)
		{
			$this->_getFile('scorers',$country);
		}		
	}
	// odds
	public function getOddsFile()
	{
		$countries = $this->TGAPI_ODDS_COUNTRIES;
		foreach($countries as $country)
		{
			$this->_getFile('odds',$country);
		}		
	}
	
	
	private function _getFile($operation,$country)
	{
		$url = self::TGAPI_ENDPOINT . $this->apikey . self::TGAPI_SPORTPREFIX . $operation . '/' . $country . '.xml';
		
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, $url);
		curl_setopt($ch, CURLOPT_HEADER, 0);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($ch, CURLOPT_TIMEOUT, 10);
		curl_setopt($ch, CURLOPT_ENCODING, '');
        $output = curl_exec($ch);
        curl_close($ch);
        
        if(!is_dir(self::TGAPI_PATH . $operation)) mkdir(self::TGAPI_PATH . $operation);
        $file = fopen(self::TGAPI_PATH . $operation . '/' . $country . '.xml', 'w');
        fwrite($file, $output);
        fclose($file);
	}
	
	
	
	private function _convertXmlToArray($xmlstring)
	{
		$xml = simplexml_load_string($xmlstring);
		$json = json_encode($xml);
		$array = json_decode($json,TRUE);
		return $array;	
	}
	
	// livescore
	public function getLivescore($country)
	{
		$operation = 'livescore';
		$file = file_get_contents(self::TGAPI_PATH . $operation . '/' . $country . '.xml');
		return $this->_convertXmlToArray($file);	
	}
	// fixtures
	public function getFixtures($country)
	{
		$operation = 'fixtures';
		$file = file_get_contents(self::TGAPI_PATH . $operation . '/' . $country . '.xml');
		return $this->_convertXmlToArray($file);
	}
	// recent_results
	public function getRecentResults($country)
	{
		$operation = 'recent_results';
		$file = file_get_contents(self::TGAPI_PATH . $operation . '/' . $country . '.xml');
		return $this->_convertXmlToArray($file);
	}
	// results
	public function getSeasonsResults($country)
	{
		$operation = 'results';
		$file = file_get_contents(self::TGAPI_PATH . $operation . '/' . $country . '.xml');
		return $this->_convertXmlToArray($file);	
	}
	// standings
	public function getStandings($country)
	{
		$operation = 'standings';
		$file = file_get_contents(self::TGAPI_PATH . $operation . '/' . $country . '.xml');
		return $this->_convertXmlToArray($file);
	}
	// scorers
	public function getScorers($country)
	{
		$operation = 'scorers';
		$file = file_get_contents(self::TGAPI_PATH . $operation . '/' . $country . '.xml');
		return $this->_convertXmlToArray($file);	
	}
	// odds
	public function getOdds($country)
	{
		if(!in_array($country, $this->TGAPI_ODDS_COUNTRIES)) return array();
		$operation = 'odds';
		$file = file_get_contents(self::TGAPI_PATH . $operation . '/' . $country . '.xml');
		return $this->_convertXmlToArray($file);
	}
	
	
}


?>