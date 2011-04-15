<?php
/**
 * @file
 * Redirected page callback file for the protected_node module.
 */

/**
 * Create the form asking the end users for the node password.
 *
 * \todo
 * It would be a good idea to transform this function in a theme() call instead.
 */
function protected_node_enterpassword() {
  // make sure we have a destination otherwise there is no password to check
  if (!isset($_GET['destination'])) {
    // Illegal call
    watchdog('protected_node', 'Illegal call to /protected-node', array(), WATCHDOG_WARNING);
    drupal_access_denied();
  }

  // some variable initialization
  $node = node_load($_SESSION['_protected_node']['current']);
  $types = node_get_types();
  $node_type = $types[$node->type];
  $has_token = module_exists('token');

  // setup the title of this page
  $title = variable_get('protected_node_title', NULL);
  if (!empty($title)) {
    if ($has_token) {
      $title = token_replace($title, 'node', $node);
    }
    drupal_set_title($title);
  }

  // information appear between the title and the password form
  $info = variable_get('protected_node_info', '');
  if ($has_token) {
    $info = token_replace($info, 'node', $node);
  }
  $form['protected_node'] = array(
    '#value' => $info,
  );

  // enter the detailed description of the protected node password
  //            array('@node' => $node_type->name,
  //                  '@title' =>  ? '"'.$node->title.'"' : '')
  $description = variable_get('protected_node_description', '');
  if (!$description) {
    // TODO: this needs to move to {protected_node} and be loaded along the other information
    // in this case we assume the user may not have the token module
    // so we do conversions of the node type and title "by hand"
    $node_title = variable_get('protected_node-'. $node->nid .'-show_title', FALSE);
    if ($node_title) {
      // embellish the title with double quotes
      $node_title = '"' . $node_title . '"';
    }
    $description = t('The @node_type @node_title you are trying to view is password protected. Please enter the password below to proceed.',
                     array('@node_type' => $node_type->name, '@node_title' => $node_title));
  }
  elseif ($has_token) {
    $description = token_replace($description, 'node', $node);
  }
  $form['protected_node_enterpassword'] = array(
    '#type' => 'fieldset',
    '#description' => $description,
    '#collapsible' => FALSE
  );

  // create the password widget
  $form['protected_node_enterpassword']['password'] = array(
    '#type' => 'password',
    '#title' => t('@node_type password', array('@node_type' => $node_type->name)),
    '#size' => 20
  );

  // add a submit button
  $form['protected_node_enterpassword']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('OK')
  );

  // add a cancel link when 'back' is defined (i.e. referer on the previous page)
  if (isset($_GET['back'])) {
    $cancel = $_GET['back'];
  }
  elseif (variable_get('protected_node_cancel', 0)) {
    $cancel = '<front>';
  }
  if ($cancel) {
    $form['protected_node_enterpassword']['cancel'] = array(
      '#value' => l('Cancel', $_GET['back']),
    );
  }

  return $form;
}

/**
 * Verify that the user entered the correct password.
 */
function protected_node_enterpassword_validate($form, &$form_state) {
  $sql = "SELECT nid FROM {protected_nodes} WHERE passwd = '%s' AND nid = %d";
  $cnt = db_result(db_query($sql, sha1($form['#post']['password']), $_SESSION['_protected_node']['current']));
  if ($cnt < 1) {
    form_set_error('password', t('Incorrect password!'));
  }
}

/**
 * Allow the user to see this node.
 */
function protected_node_enterpassword_submit($form, &$form_state) {
  $_SESSION['_protected_node']['passwords'][$_SESSION['_protected_node']['current']] = TRUE;
  unset($_SESSION['_protected_node']['current']);
}

// vim: ts=2 sw=2 et syntax=php
