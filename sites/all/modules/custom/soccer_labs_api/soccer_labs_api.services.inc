<?php


function soccer_labs_api_services_resources() {
  $resources = array(
    'soccer_labs_api_resources' => array(
      'actions' => array(
        'get_competitions' => array(
          'callback' => 'soccer_labs_api_get_competitions',
          'access arguments' => array('use api for extract info'),
        ),
        'get_teams' => array(
          'callback' => 'soccer_labs_api_get_teams',
          'access arguments' => array('use api for extract info'),
        ),
        'get_games' => array(
          'callback' => 'soccer_labs_api_get_games',
          'access arguments' => array('use api for extract info'),
        ),
		'upcoming_game' => array(
          'callback' => 'soccer_labs_api_upcoming_game', 
          'access arguments' => array('use api for extract info'),
        ),
		'recent_game' => array(
          'callback' => 'soccer_labs_api_recent_game', 
          'access arguments' => array('use api for extract info'),
        ),
		'get_answer_bot' => array(
          'callback' => 'soccer_labs_api_get_answer_bot', 
          'access arguments' => array('use api for extract info'),
        ),
      ),
    ),
  );
  return $resources;
}


function _soccer_labs_api_resource_access()
{
	return true;
}

