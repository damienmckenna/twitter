<?php
/**
 * @file
 * Page callbacks for Twitter module.
 */

/**
 * Form builder; Twitter settings form.
 */
function twitter_admin_form() {
  $form = array();

  $form['import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter import'),
    '#description' => t('Import and display the Twitter statuses of site users who have entered their Twitter account information.'),
  );

  $form['import']['twitter_import'] = array(
    '#type' => 'checkbox',
    '#title' => t('Import Twitter statuses'),
    '#default_value' => variable_get('twitter_import', TRUE),
  );

  $periods = array(0 => t('Never'));
  $periods += drupal_map_assoc(array(604800, 2419200, 7257600, 31449600), 'format_interval');
  $form['import']['twitter_expire'] = array(
    '#type' => 'select',
    '#title' => t('Delete old statuses'),
    '#default_value' => variable_get('twitter_expire', 0),
    '#options' => $periods
  );

  $form['oauth'] = array(
    '#type' => 'fieldset',
    '#title' => t('OAuth Settings'),
    '#description' => t(''),
    '#access' => module_exists('oauth_common'),
    '#description' => t('To enable OAuth based access for twitter, you must <a href="@url">register your application</a> with twitter and add the provided keys here.', array('@url' => 'https://dev.twitter.com/apps/new')),
  );

  $form['oauth']['callback_url'] = array(
    '#type' => 'item',
    '#title' => t('Callback URL'),
    '#value' => url('twitter/oauth', array('absolute' => TRUE)),
  );

  $form['oauth']['twitter_consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('OAuth Consumer key'),
    '#default_value' => variable_get('twitter_consumer_key', NULL),
  );

  $form['oauth']['twitter_consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('OAuth Consumer secret'),
    '#default_value' => variable_get('twitter_consumer_secret', NULL),
  );

  // Twitter external APIs settings.
  $form['twitter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter Settings'),
    '#description' => t('The following settings connect Twitter module with external APIs. ' .
      'Change them if, for example, you want to use Identi.ca.'),
  );
  $form['twitter']['twitter_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter host'),
    '#default_value' => variable_get('twitter_host', TWITTER_HOST),
  );
  $form['twitter']['twitter_api'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter API'),
    '#default_value' => variable_get('twitter_api', TWITTER_API),
  );
  $form['twitter']['twitter_search'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter search'),
    '#default_value' => variable_get('twitter_search', TWITTER_SEARCH),
  );
  $form['twitter']['twitter_tinyurl'] = array(
    '#type' => 'textfield',
    '#title' => t('TinyURL'),
    '#default_value' => variable_get('twitter_tinyurl', TWITTER_TINYURL),
  );

  return system_settings_form($form);
}

function twitter_user_settings($account) {
  module_load_include('inc', 'twitter');

  $output = '';
  // This is directly calling a hook implementation, which is bad and naughty,
  // but oh well. We'll fix this in the next reshuffling when user account
  // management gets an overhaul.
  $twitter_accounts = twitter_twitter_accounts($account);
  if (!empty($twitter_accounts)) {
    $output .= drupal_get_form('twitter_account_list_form', $twitter_accounts);
  }
  $output .= drupal_get_form('twitter_account_form', $account);

  return $output;
}

function twitter_account_list_form($form_state, $twitter_accounts = array()) {
  $form['#tree'] = TRUE;
  $form['accounts'] = array();

  foreach ($twitter_accounts as $twitter_account) {
    $form['accounts'][] = _twitter_account_list_row($twitter_account);
  }

  if (!empty($twitter_accounts)) {
    $form['buttons']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save changes'),
    );
  }

  return $form;
}

function _twitter_account_list_row($account) {
  $form['#account'] = $account;

  $form['id'] = array(
    '#type' => 'value',
    '#value' => $account->id,
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );

  $form['screen_name'] = array(
    '#type' => 'value',
    '#value' => $account->screen_name,
  );

  $form['image'] = array(
    '#type' => 'markup',
    '#value' => theme('image', $account->profile_image_url, '', '', array(), FALSE),
  );

  $form['visible_name'] = array(
    '#type' => 'markup',
    '#value' => l($account->screen_name, 'http://www.twitter.com/' . $account->screen_name),
  );

  $form['description'] = array(
    '#type' => 'markup',
    '#value' => filter_xss($account->description),
  );

  $form['protected'] = array(
    '#type' => 'markup',
    '#value' => empty($account->protected) ? t('No') : t('Yes'),
  );

  // Here we use user_access('import own tweets') to check permission
  // instead of user_access('import own tweets', $account->uid)
  // because we allow roles with sufficient permission to overwrite
  // the user's import settings.
  if (variable_get('twitter_import', TRUE) && user_access('import own tweets')) {
    $form['import'] = array(
      '#type' => 'checkbox',
      '#default_value' => user_access('import own tweets') ? $account->import : '',
    );
  }

  $form['delete'] = array(
    '#type' => 'checkbox',
  );

  return $form;
}

/**
 * Themes the list of Twitter accounts.
 */
function theme_twitter_account_list_form($form) {
  if (variable_get('twitter_import', TRUE) && user_access('import own tweets')) {
    $header = array('', t('Name'), t('Description'), t('Private'), t('Import'), t('Delete'));
  } 
  else {
    $header = array('', t('Name'), t('Description'), t('Private'), t('Delete'));
  }

  if (user_access('make twitter accounts global')) {
    $header[] = '';
  }

  $rows = array();

  foreach (element_children($form['accounts']) as $key) {
    $element = &$form['accounts'][$key];
    if (variable_get('twitter_import', TRUE) && user_access('import own tweets')) {
      $row = array(
      drupal_render($element['image']),
      drupal_render($element['id']) . drupal_render($element['screen_name']) . drupal_render($element['visible_name']),
      drupal_render($element['description']),
      drupal_render($element['protected']),
      drupal_render($element['import']),
      drupal_render($element['delete']),
      );
    } 
    else {
      $row = array(
      drupal_render($element['image']),
      drupal_render($element['id']) . drupal_render($element['screen_name']) . drupal_render($element['visible_name']),
      drupal_render($element['description']),
      drupal_render($element['protected']),
      drupal_render($element['delete']),
      );
    }

    if (user_access('make twitter accounts global')) {
      $label = ($element['#account']->is_global) ? t('remove global') : t('make global');
      $row[] = l($label, 'user/'. $element['#account']->uid .'/edit/twitter/global/'. $element['#account']->id);
    }

    $rows[] = $row;
  }

  $output = theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;
}

/**
 * Form submit handler for altering the list of Twitter accounts.
 */
function twitter_account_list_form_submit($form, &$form_state) {
  $accounts = $form_state['values']['accounts'];
  foreach($accounts as $account) {
    if (empty($account['delete'])) {
      twitter_account_save($account);
      drupal_set_message(t('The Twitter account settings were updated.'));
    }
    else {
      twitter_user_delete($account['id']);
      drupal_set_message(t('The Twitter account was deleted.'));
    }
  }
}

/**
 * Form builder for setting a Twitter account as global.
 */
function twitter_user_make_global($form_state, $account, $twitter_uid) {
  module_load_include('inc', 'twitter');

  $twitter_account = twitter_account_load($twitter_uid);

  $form = array();

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );

  $form['twitter_uid'] = array(
    '#type' => 'value',
    '#value' => $twitter_uid,
  );

  if ($twitter_account->is_global) {
    $text = t('Are you sure you want to remove %screen_name from the global accounts?', array('%screen_name' => $twitter_account->screen_name));
    $description = t('This means other users will no longer be allowed to post using this account.');
  }
  else {
    $text = t('Are you sure you want to allow other users to access the %screen_name account?', array('%screen_name' => $twitter_account->screen_name));
    $description = t('This will allow other users to post using this account.');
  }

  return confirm_form($form, $text, 'user/'. $account->uid .'/edit/twitter', $description);
}

function twitter_user_make_global_submit($form, &$form_state) {
  db_query("UPDATE {twitter_account} SET is_global = (1 - is_global) WHERE twitter_uid = %d", $form_state['values']['twitter_uid']);

  $form_state['redirect'] = 'user/'. $form_state['values']['uid'] .'/edit/twitter';
}

/**
 * Form to add a Twitter account.
 */
function twitter_account_form($form_state, $account = NULL) {
  if (empty($account)) {
    global $user;
    $account = $user;
  }

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add account'),
  );

  return $form;
}

/**
 * Form validation for adding a new Twitter account.
 */
function twitter_account_form_validate($form, &$form_state) {
  $key = variable_get('twitter_consumer_key', '');
  $secret = variable_get('twitter_consumer_secret', '');
  if ($key == '' || $secret == '') {
    form_set_error('', t('Please configure your consumer key and secret key at <a href="!url">Twitter settings</a>.',
      array('!url' => url('admin/settings/twitter'))));
  }
}

/**
 * Implements hook_FORM_ID_submit()
 *
 * Loads Twitter account details and adds them to the user account
 */
function twitter_account_form_submit($form, &$form_state) {
  $key = variable_get('twitter_consumer_key', '');
  $secret = variable_get('twitter_consumer_secret', '');
  $twitter = new Twitter6($key, $secret);
  $token = $twitter->get_request_token();

  if ($token) {
    $_SESSION['twitter_oauth']['account'] = user_load($form['uid']['#value']);
    $_SESSION['twitter_oauth']['token'] = $token;
    $_SESSION['twitter_oauth']['destination'] = $_GET['q'];
    drupal_goto($twitter->get_authorize_url($token));
  }
  else {
    drupal_set_message(t('Could not obtain a valid token from the Twitter API. Please review the configuration.'),
                       'error');
  }
}

/**
 * Callback to process the response from the Twitter authentication.
 */
function twitter_oauth_callback(&$form_state) {
  if (isset($_GET['denied']) || empty($_GET['oauth_token'])) {
    drupal_set_message(t('The connection to Twitter failed. Please try again.'), 'error');
    global $user;
    if ($user->uid) {
      // User is logged in, was attempting to OAuth a Twitter account.
      drupal_goto('user/' . $user->uid . '/edit/twitter');
    }
    else {
      // Anonymous user, redirect to front page.
      drupal_goto('<front>');
    }
  }

  $form['#post']['oauth_token'] = $_GET['oauth_token'];
  $form['oauth_token'] = array(
    '#type' => 'hidden',
    '#default_value' => $_GET['oauth_token'],
  );

  return $form;
}

function twitter_oauth_callback_validate($form, &$form_state) {
  module_load_include('inc', 'twitter');
  $key = variable_get('twitter_consumer_key', '');
  $secret = variable_get('twitter_consumer_secret', '');

  $form_state['twitter_oauth'] = $_SESSION['twitter_oauth'];
  unset($_SESSION['twitter_oauth']);

  $token = $form_state['twitter_oauth']['token'];
  if (!is_array($token) || !$key || !$secret) {
    form_set_error('oauth_token', t('Invalid Twitter OAuth request'));
  }

  if ($token['oauth_token'] != $form_state['values']['oauth_token']) {
    form_set_error('oauth_token', t('Invalid OAuth token.'));
  }

  $twitter = new Twitter6($key, $secret, $token['oauth_token'], $token['oauth_token_secret']);
  $response = $twitter->get_access_token();
  $form_state['twitter_oauth']['response'] = $response;
}

/**
 * Implements hook_form_FORM_ID_submit()
 *
 * Attaches a Twitter account to a Drupal user.
 */
function twitter_oauth_callback_submit(&$form, &$form_state) {
  $key = variable_get('twitter_consumer_key', '');
  $secret = variable_get('twitter_consumer_secret', '');
  $response = $form_state['twitter_oauth']['response'];

  $twitter = new Twitter6($key, $secret, $response['oauth_token'], $response['oauth_token_secret']);
  $twitter_account = $twitter->users_show($response['screen_name']);
  $twitter_account->set_auth($response);
  $account = $form_state['twitter_oauth']['account'];
  twitter_account_save($twitter_account, TRUE, $account);
  $form_state['redirect'] = $form_state['twitter_oauth']['destination'];
  $form['#programmed'] = FALSE;
}
