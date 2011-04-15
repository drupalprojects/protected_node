<?php
/**
 * @file
 * Configuration file for the protected_node module.
 */

/**
 * Define the settings form
 *
 * @return $form The settings form
 */
function protected_node_admin_settings() {
  $form['protected_node_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Protected node form'),
  );

  $form['protected_node_form']['protected_node_cancel'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always add a cancel link'),
    '#default_value' => variable_get('protected_node_cancel', 0),
    '#description' => t('Whether a cancel link should be added to the password form. If checked and we do not have a back link, then the cancel is set to &lt;front&gt; instead.'),
  );

  $form['protected_node_form']['protected_node_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Password page title'),
    '#default_value' => variable_get('protected_node_title', 'Protected page -- Enter password'),
    '#description' => t('Enter the title of the protected node page. No HTML allowed. You can use node type tokens from the token module when installed.'),
  );

  $form['protected_node_form']['protected_node_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Password page general information'),
    '#default_value' => variable_get('protected_node_info', ''),
    '#description' => t('Enter general information for the protected node page. HTML is accepted. You can use node type tokens from the token module when installed.'),
  );

  $form['protected_node_form']['protected_node_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Password page description (inside the the fieldset)'),
    '#default_value' => variable_get('protected_node_description', ''),
    '#description' => t('Enter specific description for the protected node page. This description is displayed inside the fieldset. HTML is accepted. You can use node type tokens from the token module when installed.'),
  );

  if (module_exists('token')) {
    $form['protected_node_form']['protected_node_tokens'] = array(
      '#value' => theme('token_tree', array('global', 'node')),
    );
  }

  return system_settings_form($form);
}

// vim: ts=2 sw=2 et syntax=php
