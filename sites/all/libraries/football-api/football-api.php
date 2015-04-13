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
  throw new Exception('Football api needs the CURL PHP extension.');
}
if (!function_exists('json_decode')) {
  throw new Exception('Football api needs the JSON PHP extension.');
}

class FootballApi 
{
	const FAPI_ENDPOINT = 'http://football-api.com/api/?';
	
	public function __construct($apikey) {
		$this->apikey = $apikey;
		$this->APIRequestsRemaining = 0;
	}
	
	public function getCompetitions()
	{
		$competitions = $this->_makeCall(array('Action' => 'competitions'));
		return ( isset($competitions->Competition) ? $competitions->Competition : array() ) ;
	}
	
	public function getStandings($comp_id)
	{
		$standings = $this->_makeCall(array('Action' => 'standings', 'comp_id' => $comp_id));
		return ( isset($standings->teams) ? $standings->teams : array() ) ;
	}
	
	public function getTodayMatches($comp_id)
	{
		$matches = $this->_makeCall(array('Action' => 'today', 'comp_id' => $comp_id));
		return ( isset($matches->matches) ? $matches->matches : array() ) ;
	}
	
	public function getTodayMatchesPastWithFixtures($comp_id)
	{
		$from = new DateTime();
		$to = new DateTime();
		$from = $from->modify( '-6 hours' );
		$to = $to->modify( '+6 hours' );
		return $this->getFixtures($comp_id, '', $from->format('d.m.Y'), $to->format('d.m.Y'));
	}
	
	public function getAllPossibleFixtures($comp_id)
	{
		$from = new DateTime();
		$to = new DateTime();
		$from = $from->modify( '-7 day' );
		return $this->getFixtures($comp_id, '', $from->format('d.m.Y'), $to->format('d.m.Y'));
	}
	
	public function getNextFixtures($comp_id)
	{
		$from = new DateTime();
		$to = new DateTime();
		$to = $to->modify( '+7 day' );
		return $this->getFixtures($comp_id, '', $from->format('d.m.Y'), $to->format('d.m.Y'));
	}
	
	public function getLastFixtures($comp_id)
	{
		$from = new DateTime();
		$to = new DateTime();
		$from = $from->modify( '-7 day' );
		return $this->getFixtures($comp_id, '', $from->format('d.m.Y'), $to->format('d.m.Y'));
	}
	
	public function getFixtures($comp_id, $match_date = '', $from_date = '', $to_date = '')
	{
		$params = array('Action' => 'fixtures', 'comp_id' => $comp_id);
		
		if($match_date!='') $params['match_date'] = $match_date;
		if($from_date!='') $params['from_date'] = $from_date;
		if($to_date!='') $params['to_date'] = $to_date;
		$matches = $this->_makeCall($params);
		return ( isset($matches->matches) ? $matches->matches : array() ) ;
	}
	
	public function getMatchCommentaries($match_id)
	{
		$commentaries = $this->_makeCall(array('Action' => 'commentaries', 'match_id' => $match_id));
		return ( isset($commentaries->commentaries) ? $commentaries->commentaries : array() ) ;
	}
	
	private function _makeCall($params)
	{
		$url = self::FAPI_ENDPOINT . 'APIKey=' . $this->apikey;
		foreach($params as $key => $value) $url .= "&" . $key . '=' . $value;
		
		//watchdog('debug_call',$url);
		
		$ch = curl_init($url);
        curl_setopt($ch, CURLOPT_HEADER, 0);
        curl_setopt($ch, CURLOPT_POST, 0);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        $output = curl_exec($ch);       
        curl_close($ch);
        //watchdog('debug_call',$output);
        $return = json_decode($output); 
        $this->APIRequestsRemaining = $return->APIRequestsRemaining;
        return $return;
	}
}

?>